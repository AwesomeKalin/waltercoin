// Copyright (c) 2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WALTERCOIN_TEST_FUZZ_MEMPOOL_UTILS_H
#define WALTERCOIN_TEST_FUZZ_MEMPOOL_UTILS_H

#include <validation.h>

class DummyChainState final : public Chainstate
{
public:
    void SetMempool(CTxMemPool* mempool)
    {
        m_mempool = mempool;
    }
};

#endif // WALTERCOIN_TEST_FUZZ_MEMPOOL_UTILS_H