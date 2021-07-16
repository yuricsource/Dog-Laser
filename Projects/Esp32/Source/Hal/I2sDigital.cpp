
#include "Hardware.h"
#include "I2sDigital.h"
#include "DebugAssert.h"
#include "Dwt.h"
#include "TimeLimit.h"

#define I2S_DEBUG

namespace Hal
{

I2sDigital::I2sDigital(Gpio *gpio, Gpio::GpioIndex i2sPin, I2sBus i2sBus) :
		_gpio(gpio), _i2sPin(i2sPin), _i2s(i2sBus), _bitSample(I2sBitSample::Sample16Bits),  _sampleRate(0),
		_bufferSize(0), _dmaBuffer(nullptr), _autoTrigger(false)
{

}

bool I2sDigital::Init( I2sBitSample bitSample, uint32_t bufferSize,
						uint32_t sampleRate, bool autoTrigger)
{
	// Calulate how many DMA Buffer is needed
	_bufferCounts = bufferSize / BytesPerBuffer;
	_bufferSize = bufferSize;
	if (_bufferSize % BytesPerBuffer != 0)
		_bufferCounts += 1;
	
	_dmaBuffer = reinterpret_cast<lldesc_t*>(heap_caps_malloc(sizeof(lldesc_t) * _bufferCounts, MALLOC_CAP_DMA));
	assert(_dmaBuffer != nullptr);
	
	if (_dmaBuffer == nullptr)
		return false;
	
	for (uint8_t i = 0; i < _bufferCounts; i ++)
	{
		// Initialise all DMA Buffers
		_dmaBuffer[i].owner = 1;
		_dmaBuffer[i].sosf = 0;
		_dmaBuffer[i].buf = reinterpret_cast<uint8_t*>(heap_caps_malloc(BytesPerBuffer, MALLOC_CAP_DMA));
		assert(_dmaBuffer[i].buf != nullptr);
		_dmaBuffer[i].length = BytesPerBuffer;
		assert(_dmaBuffer[i].length == BytesPerBuffer);
		_dmaBuffer[i].size = BytesPerBuffer;
		assert(_dmaBuffer[i].size == BytesPerBuffer);
		_dmaBuffer[i].offset = 0;
		_dmaBuffer[i].empty = 0;
		_dmaBuffer[i].eof = 1;

		// Link the DMB buffers
		if (i == (_bufferCounts - 1))
			_dmaBuffer[i].qe.stqe_next = &_dmaBuffer[i + 1];
	}

	// Make a cricular buffer, linking the last buffer with the first buffer
	if (_autoTrigger)
		_dmaBuffer[_bufferCounts - 1].qe.stqe_next = _dmaBuffer[0].qe.stqe_next;

#ifdef I2S_DEBUG
	printf("I2S Init. BytesPerBuffer:%d, bufferSize: %d, _bufferCounts:%d, _autoTrigger:%d\n", BytesPerBuffer, bufferSize, _bufferCounts, _autoTrigger);
#endif

	gpio_matrix_out(static_cast<uint32_t>(_i2sPin), (_i2s == I2sBus::I2s0) ? I2S0O_DATA_OUT0_IDX : I2S1O_DATA_OUT0_IDX, false, false);

	volatile i2s_dev_t *i2sPeripheral = (_i2s == I2sBus::I2s0) ? &I2S0: &I2S1;
	// Toogle the Tx and Rx resets
	i2sPeripheral->conf.tx_reset = 1;
	i2sPeripheral->conf.tx_reset = 0;
	i2sPeripheral->conf.rx_reset = 1;
	i2sPeripheral->conf.rx_reset = 0;
	// Toogle the Tx and Rx FIFO resets
	i2sPeripheral->conf.rx_fifo_reset = 1;
	i2sPeripheral->conf.rx_fifo_reset = 0;
	i2sPeripheral->conf.tx_fifo_reset = 1;
	i2sPeripheral->conf.tx_fifo_reset = 0;
	// Toogle the in and out buffers resets
	i2sPeripheral->lc_conf.in_rst = 1;
	i2sPeripheral->lc_conf.in_rst = 0;
	i2sPeripheral->lc_conf.out_rst = 1;
	i2sPeripheral->lc_conf.out_rst = 0;
	// LCD enabled = false, used in parallel mode
	i2sPeripheral->conf2.val = 0;
	i2sPeripheral->conf2.lcd_en = 0;
	// Configurate DMA FIFO
	i2sPeripheral->fifo_conf.val = 0;
	i2sPeripheral->fifo_conf.tx_fifo_mod_force_en = 1;
	i2sPeripheral->fifo_conf.tx_fifo_mod = 2;
	i2sPeripheral->fifo_conf.tx_data_num = 32;
	i2sPeripheral->fifo_conf.dscr_en = 1;
	i2sPeripheral->conf_chan.val = 0;
	i2sPeripheral->conf_chan.tx_chan_mod = 0;
	i2sPeripheral->conf1.val = 0;
	i2sPeripheral->conf1.tx_stop_en = 0;
	i2sPeripheral->conf1.tx_pcm_bypass = 1;
	i2sPeripheral->timing.val = 0;
	i2sPeripheral->conf.tx_right_first = 1;
	i2sPeripheral->conf.tx_msb_right = 1;
	i2sPeripheral->conf.tx_msb_shift = 0;
	i2sPeripheral->conf.tx_mono = 0;
	i2sPeripheral->conf.tx_short_sync = 0;
	
	const uint32_t lc_conf_reset_flags = I2S_IN_RST_M | I2S_OUT_RST_M | I2S_AHBM_RST_M | I2S_AHBM_FIFO_RST_M;
	i2sPeripheral->lc_conf.val |= lc_conf_reset_flags;
	i2sPeripheral->lc_conf.val &= ~lc_conf_reset_flags;

	const uint32_t conf_reset_flags = I2S_RX_RESET_M | I2S_RX_FIFO_RESET_M | I2S_TX_RESET_M | I2S_TX_FIFO_RESET_M;
	i2sPeripheral->conf.val |= conf_reset_flags;
	i2sPeripheral->conf.val &= ~conf_reset_flags;
	while (i2sPeripheral->state.rx_fifo_reset_back);

	i2sPeripheral->lc_conf.val    = I2S_OUT_DATA_BURST_EN | I2S_OUTDSCR_BURST_EN;
	//dmaBufferDescriptorActive = 0;
	i2sPeripheral->out_link.addr = (uint32_t)&_dmaBuffer[0];
	i2sPeripheral->out_link.start = 1;
	i2sPeripheral->int_clr.val = i2sPeripheral->int_raw.val;
	i2sPeripheral->int_ena.val = 0;
	
	//start transmission
	i2sPeripheral->conf.tx_start = 1;
	return true;
}

void I2sDigital::Refresh()
{
	volatile i2s_dev_t *i2sPeripheral = (_i2s == I2sBus::I2s0) ? &I2S0: &I2S1;
	// DEBUG_PRINTLN("I2S TX");
	// esp_intr_disable(interruptHandle);
	i2sPeripheral->lc_conf.val |= (I2S_IN_RST_M | I2S_OUT_RST_M | I2S_AHBM_RST_M | I2S_AHBM_FIFO_RST_M);
	i2sPeripheral->lc_conf.val &= ~(I2S_IN_RST_M | I2S_OUT_RST_M | I2S_AHBM_RST_M | I2S_AHBM_FIFO_RST_M);

	i2sPeripheral->conf.val |= (I2S_RX_RESET_M | I2S_RX_FIFO_RESET_M | I2S_TX_RESET_M | I2S_TX_FIFO_RESET_M);
	i2sPeripheral->conf.val &= ~(I2S_RX_RESET_M | I2S_RX_FIFO_RESET_M | I2S_TX_RESET_M | I2S_TX_FIFO_RESET_M);
	TimeLimit timeout = {};

	while (i2sPeripheral->state.rx_fifo_reset_back)
	{
		Dwt::DelayMicrosecond(10);
		if (timeout.IsTimeUp(100))
		{
			DebugAssertFail("Timeout to reset TX Fifo");
		}
	}

    i2sPeripheral->lc_conf.val    = I2S_OUT_DATA_BURST_EN | I2S_OUTDSCR_BURST_EN;
	i2sPeripheral->out_link.addr = (uint32_t)&_dmaBuffer[0];
	i2sPeripheral->out_link.start = 1;
	i2sPeripheral->int_clr.val = i2sPeripheral->int_raw.val;
	i2sPeripheral->int_ena.val = 0;

	// Start sending
	i2sPeripheral->conf.tx_start = 1;
}


bool I2sDigital::UpdateBuffer(uint8_t *buffer, uint32_t size, uint32_t offset)
{
	if (buffer == nullptr || size == 0 || size + offset > _bufferSize)
		return false;

	// Seek the offsets and buffers chunks in the buffer array
	uint16_t bufferIndex = offset / BytesPerBuffer;
	uint16_t firstBytes = offset % BytesPerBuffer;
	uint16_t buffersCount = (offset + size) / BytesPerBuffer;
	uint16_t bytesLeft = (offset + size) % BytesPerBuffer;
#ifdef I2S_DEBUG
		printf("-----------------------------------------------------------------------------------------------\n");
#endif
	// Copy the first partial buffer
	if (firstBytes != 0)
	{
		uint8_t firstCopySize = std::min(BytesPerBuffer - firstBytes, size);
		memcpy((void*)(_dmaBuffer[bufferIndex].buf + firstBytes), (void*)buffer, firstCopySize);
#ifdef I2S_DEBUG
		printf("Buffer first parcial buffer: Buffer DMA %d, offset %d, Size %d\n", bufferIndex, firstBytes, firstCopySize);
#endif
		bufferIndex++;
		buffer += firstBytes;
		if (firstCopySize != BytesPerBuffer - firstBytes)
		 return true;
	}
	// Copy all whole buffers
	for (; bufferIndex < buffersCount; bufferIndex++)
	{
		memcpy((void*)_dmaBuffer[bufferIndex].buf, (void*)buffer, BytesPerBuffer);
		buffer += BytesPerBuffer;
#ifdef I2S_DEBUG
		printf("Full Buffer copy: Buffer DMA %d, Size %d\n", bufferIndex, BytesPerBuffer);
#endif
	}
	// If there is a last partial buffer
	if (bytesLeft != 0)
	{
		memcpy((void*)_dmaBuffer[bufferIndex].buf, (void*)buffer, bytesLeft);
#ifdef I2S_DEBUG
		printf("Buffer Last Partical buffer: Buffer DMA %d, Size %d\n", bufferIndex, bytesLeft);
#endif
	}

	return true;
}

I2sDigital::~I2sDigital()
{
}

bool I2sDigital::UpdateChannelClock(I2sBitSample sampleBit, uint32_t sampleRate)
{
	if (_enabled == false)
		return false;

	_bitSample = sampleBit;
	_sampleRate = sampleRate;

#ifdef I2S_DEBUG
	printf("Set new Sample Rate: %d\n", _sampleRate);
#endif

	return true;
}

void I2sDigital::Start(Gpio::GpioIndex data)
{
	if (_enabled)
		return;

#ifdef I2S_DEBUG
	printf("Configuring I2s(%d): sample rate %d, bitSample: %d on pin %d\n", static_cast<uint8_t>(_i2s),
		_sampleRate, static_cast<uint8_t>(_bitSample), static_cast<uint8_t>(data));
#endif

	_enabled = true;
}

void I2sDigital::Stop()
{
	if (_enabled)
	{
		_enabled = false;
	}
}

bool I2sDigital::Send(const uint8_t *buffer, uint32_t size, uint32_t timeout)
{

	if (buffer == nullptr || size == 0)
		return false;

	return true;
}

} // namespace Hal
