// Copyright (c) 2016-2020 The Waltercoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WALTERCOIN_QT_TEST_RPCNESTEDTESTS_H
#define WALTERCOIN_QT_TEST_RPCNESTEDTESTS_H

#include <QObject>
#include <QTest>

namespace interfaces {
class Node;
} // namespace interfaces

class RPCNestedTests : public QObject
{
public:
    explicit RPCNestedTests(interfaces::Node& node) : m_node(node) {}
    interfaces::Node& m_node;

    Q_OBJECT

    private Q_SLOTS:
    void rpcNestedTests();
};

#endif // WALTERCOIN_QT_TEST_RPCNESTEDTESTS_H
