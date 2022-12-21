// Copyright (c) 2011-2020 The Waltercoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WALTERCOIN_QT_WALTERCOINADDRESSVALIDATOR_H
#define WALTERCOIN_QT_WALTERCOINADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class WaltercoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit WaltercoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** Waltercoin address widget validator, checks for a valid waltercoin address.
 */
class WaltercoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit WaltercoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // WALTERCOIN_QT_WALTERCOINADDRESSVALIDATOR_H
