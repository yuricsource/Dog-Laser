#pragma once

#include "HalCommon.h"

namespace Status
{

class BoardStatus
{
public:
    BoardStatus();
    ~BoardStatus();

private:

struct BoardStatusData
{
    bool BoardOnline : 1;
    uint32_t Reserved0 : 31;
} _status;
static_assert(sizeof(BoardStatusData) == 4, "BoardStatusData has wrong size, compiler error");

private:
    /// @brief	Hide Copy constructor.
    BoardStatus(const BoardStatus &) = delete;

    /// @brief	Hide Assignment operator.
    BoardStatus &operator=(const BoardStatus &) = delete;

    /// @brief	Hide Move constructor.
    BoardStatus(BoardStatus &&) = delete;

    /// @brief	Hide Move assignment operator.
    BoardStatus &operator=(BoardStatus &&) = delete;
};

} // namespace Configuration