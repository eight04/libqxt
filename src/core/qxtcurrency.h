
/****************************************************************************
** Copyright (c) 2006 - 2012, the LibQxt project.
** See the Qxt AUTHORS file for a list of authors and copyright holders.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the LibQxt project nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
** <http://libqxt.org>  <foundation@libqxt.org>
*****************************************************************************/

#ifndef QXTCURRENCY_H
#define QXTCURRENCY_H
#include "qxtglobal.h"
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <limits>
#include <QVariant>
#include <QTextStream>

//////////////////////////////////////////////////////////////////////////////
// Supporting functions

#ifndef QT_NO_DEBUG
class QDebug;
#endif

//////////////////////////////////////////////////////////////////////////////
// QxtCurrency -- fixed-precision datatype

class QXT_CORE_EXPORT QxtCurrency
{
public:
    // Construct as zero.
    QxtCurrency()
    {
	value = 0LL;
    }
    // Construct via a double.
    QxtCurrency(double v)
    {
	value = qRound64(v * 10000.0);
    }
    // Construct via an integer.
    QxtCurrency(int v)
    {
	value = static_cast<qlonglong>(v) * 10000LL;
    }
    // Construct via a 8-byte integer.
    explicit QxtCurrency(qlonglong v)
    {
	value = v;
    }
    // Construct from a string representation
    QxtCurrency(const QString &);
    QxtCurrency(const char *);

    // Conversions
    // Convert to a boolean (test for non-zero)
    operator bool() const;
    // Convert to a double.
    operator double() const;
    // Convert to an integer.
    operator int() const;
    // Convert to a string representation
    QByteArray toString() const;
    inline QString toQString() const
    {
	return QString::fromLatin1(toString());
    }
    // Extract from a QVariant
    static QxtCurrency fromVariant(const QVariant &);

    // Arithmetic
    // Unary negation
    friend QxtCurrency operator-(const QxtCurrency &rhs);
    // Add two currency values
    QxtCurrency operator+(const QxtCurrency &rhs) const
    {
	QxtCurrency result;
	result.value = value + rhs.value;
	return result;
    }
    // Add an integer to a currency value
    QxtCurrency operator+(int rhs) const
    {
	QxtCurrency result;
	result.value = value + static_cast<qlonglong>(rhs) * 10000;
	return result;
    }
    // Add a double to a currency value
    QxtCurrency operator+(double rhs) const
    {
	QxtCurrency result;
	result.value = value + qRound64(rhs * 10000);
	return result;
    }
    // Add a currency value in place
    QxtCurrency& operator+=(const QxtCurrency &rhs)
    {
	value += rhs.value;
	return *this;
    }
    // Subtract two currency values
    QxtCurrency operator-(const QxtCurrency &rhs) const
    {
	QxtCurrency result;
	result.value = value - rhs.value;
	return result;
    }
    // Subtract an integer from a currency value
    QxtCurrency operator-(int rhs) const
    {
	QxtCurrency result;
	result.value = value - static_cast<qlonglong>(rhs) * 10000LL;
	return result;
    }
    // Subtract a double from a currency value
    QxtCurrency operator-(double rhs) const
    {
	QxtCurrency result;
	result.value = value - qRound64(rhs * 10000.0);
	return result;
    }
    // Subtract a currency value in place
    QxtCurrency& operator-=(const QxtCurrency &rhs)
    {
	value -= rhs.value;
	return *this;
    }
    // Multiply two currency values
    QxtCurrency operator*(const QxtCurrency &rhs) const
    {
	QxtCurrency result;
	result.value = value * rhs.value / 10000LL;
	return result;
    }
    // Multiply a currency value by an integer
    QxtCurrency operator*(int rhs) const
    {
	QxtCurrency result;
	result.value = value * static_cast<qlonglong>(rhs);
	return result;
    }
    // Multiply a currency value by a double
    QxtCurrency operator*(double rhs) const
    {
	QxtCurrency result;
	result.value = qRound64(value * rhs);
	return result;
    }
    // Multiply a currency value in place
    QxtCurrency& operator*=(const QxtCurrency &rhs)
    {
	value = value * rhs.value / 10000LL;
	return *this;
    }
    // Divide two currency values
    QxtCurrency operator/(const QxtCurrency &rhs) const
    {
	QxtCurrency result;
	result.value = value / rhs.value * 10000LL;
	return result;
    }
    // Divide a currency value by an integer
    QxtCurrency operator/(int rhs) const
    {
	QxtCurrency result;
	result.value = value / static_cast<qlonglong>(rhs);
	return result;
    }
    // Divide a currency value by a double
    QxtCurrency operator/(double rhs) const
    {
	QxtCurrency result;
	result.value = qRound64(double(value) / rhs);
	return result;
    }
    // Divide a currency value in place
    QxtCurrency& operator/=(const QxtCurrency &rhs)
    {
	value = value / rhs.value * 10000LL;
	return *this;
    }

    // Comparison
    friend bool operator<(const QxtCurrency &lhs, const QxtCurrency &rhs);
    friend bool operator>(const QxtCurrency &lhs, const QxtCurrency &rhs);
    friend bool operator<=(const QxtCurrency &lhs, const QxtCurrency &rhs);
    friend bool operator>=(const QxtCurrency &lhs, const QxtCurrency &rhs);
    friend bool operator==(const QxtCurrency &lhs, const QxtCurrency &rhs);
    friend bool operator!=(const QxtCurrency &lhs, const QxtCurrency &rhs);
    friend bool operator<(const QxtCurrency &lhs, int rhs);
    friend bool operator>(const QxtCurrency &lhs, int rhs);
    friend bool operator<=(const QxtCurrency &lhs, int rhs);
    friend bool operator>=(const QxtCurrency &lhs, int rhs);
    friend bool operator==(const QxtCurrency &lhs, int rhs);
    friend bool operator!=(const QxtCurrency &lhs, int rhs);
    friend bool operator<(const QxtCurrency &lhs, double rhs);
    friend bool operator>(const QxtCurrency &lhs, double rhs);
    friend bool operator<=(const QxtCurrency &lhs, double rhs);
    friend bool operator>=(const QxtCurrency &lhs, double rhs);
    friend bool operator==(const QxtCurrency &lhs, double rhs);
    friend bool operator!=(const QxtCurrency &lhs, double rhs);

    // Miscellany

    // Get absolute value
    QxtCurrency abs() const;
    // Clamp value to a range
    QxtCurrency & clamp(const QxtCurrency &l, const QxtCurrency &h);
    // Clamp value to a range
    QxtCurrency clamped(const QxtCurrency &l, const QxtCurrency &h) const;
    // Round a value
    QxtCurrency round(int n=2) const;
    // Get the sign of the value
    inline int sign() const
    {
	return (*this < 0) ? -1 : ((*this > 0) ? 1 : 0);
    }
    // Insert into output stream
    friend QTextStream & operator<<(QTextStream &strm, const QxtCurrency &v)
    {
	strm << v.toString();
	return strm;
    }
    // Insert into output stream
    template<typename _CharT, typename _Traits>
    friend std::basic_ostream<_CharT, _Traits> & operator<<(
	    std::basic_ostream<_CharT, _Traits> &strm, const QxtCurrency &v)
    {
	strm << v.toString().constData();
	return strm;
    }

    friend QDataStream &operator<<(QDataStream &out, const QxtCurrency &v);
    friend QDataStream &operator>>(QDataStream &in, QxtCurrency &v);

    // Determine amortized payment amount
    static QxtCurrency amortizedPayment(const QxtCurrency& P, double r, int n);
    inline QxtCurrency amortizedPayment(double r, int n) const
    {
	return amortizedPayment(*this, r, n);
    }
    typedef QPair<QxtCurrency,QxtCurrency> Pair;
    // Determine total amortized interest amount and final payment
    static Pair amortizedInterest(QxtCurrency P, double r, int n,
	    const QxtCurrency &p);
    inline Pair amortizedInterest(double r, int n, const QxtCurrency &p) const
    {
	return amortizedInterest(*this, r, n, p);
    }
    // Generate amortization schedule
    static QList<Pair> amortize(QxtCurrency P, double r, int n,
	    QxtCurrency p=-1);
    inline QList<Pair> amortize(double r, int n, QxtCurrency p=-1) const
    {
	return amortize(*this, r, n, p);
    }

    // Actual value
    qlonglong value;

private:
    //! Parse from text
    void parseASCII(const char *);
};
Q_DECLARE_METATYPE(QxtCurrency)
#ifndef QT_NO_DEBUG
QDebug operator<<(QDebug dbg, const QxtCurrency &v);
#endif

inline QxtCurrency QxtCurrency::abs() const
{
    return (*this < 0) ? -*this : *this;
}

inline QxtCurrency::operator bool() const
{
    return value ? true : false;
}

inline QxtCurrency::operator double() const
{
    return double(value) / 10000.0;
}

inline QxtCurrency::operator int() const
{
    return int(value / 10000LL);
}

inline QxtCurrency operator-(const QxtCurrency &rhs)
{
    QxtCurrency result;
    result.value = -rhs.value;
    return result;
}

inline bool operator<(const QxtCurrency &lhs, const QxtCurrency &rhs)
{
    return lhs.value < rhs.value;
}

inline bool operator>(const QxtCurrency &lhs, const QxtCurrency &rhs)
{
    return lhs.value > rhs.value;
}

inline bool operator<=(const QxtCurrency &lhs, const QxtCurrency &rhs)
{
    return lhs.value <= rhs.value;
}

inline bool operator>=(const QxtCurrency &lhs, const QxtCurrency &rhs)
{
    return lhs.value >= rhs.value;
}

inline bool operator==(const QxtCurrency &lhs, const QxtCurrency &rhs)
{
    return lhs.value == rhs.value;
}

inline bool operator!=(const QxtCurrency &lhs, const QxtCurrency &rhs)
{
    return lhs.value != rhs.value;
}

inline bool operator<(const QxtCurrency &lhs, int rhs)
{
    return lhs.value < (rhs * 10000LL);
}

inline bool operator>(const QxtCurrency &lhs, int rhs)
{
    return lhs.value > (rhs * 10000LL);
}

inline bool operator<=(const QxtCurrency &lhs, int rhs)
{
    return lhs.value <= (rhs * 10000LL);
}

inline bool operator>=(const QxtCurrency &lhs, int rhs)
{
    return lhs.value >= (rhs * 10000LL);
}

inline bool operator==(const QxtCurrency &lhs, int rhs)
{
    return lhs == QxtCurrency(rhs);
}

inline bool operator!=(const QxtCurrency &lhs, int rhs)
{
    return lhs != QxtCurrency(rhs);
}

inline bool operator<(const QxtCurrency &lhs, double rhs)
{
    return lhs.value < rhs * 10000.0;
}

inline bool operator>(const QxtCurrency &lhs, double rhs)
{
    return lhs.value > rhs * 10000.0;
}

inline bool operator<=(const QxtCurrency &lhs, double rhs)
{
    return lhs.value <= rhs * 10000.0;
}

inline bool operator>=(const QxtCurrency &lhs, double rhs)
{
    return lhs.value >= rhs * 10000.0;
}

inline bool operator==(const QxtCurrency &lhs, double rhs)
{
    return lhs == QxtCurrency(rhs);
}

inline bool operator!=(const QxtCurrency &lhs, double rhs)
{
    return lhs != QxtCurrency(rhs);
}

namespace std {
    inline QxtCurrency abs(const QxtCurrency &v)
    {
	return v.abs();
    }
    /*! Numeric limits information for QiCurrency class. Essentially the
     *	same as std::numeric_limits<long long>
     */
    template<>
    struct numeric_limits<QxtCurrency>
    {
      static const bool is_specialized = true;

      static QxtCurrency min() throw()
      {
	  return QxtCurrency(numeric_limits<qlonglong>::min());
      }
      static QxtCurrency max() throw()
      {
	  return QxtCurrency(numeric_limits<qlonglong>::max());
      }

      static const int digits = numeric_limits<qlonglong>::digits;
      static const int digits10 = numeric_limits<qlonglong>::digits10;
      static const bool is_signed = true;
      static const bool is_integer = true;
      static const bool is_exact = true;
      static const int radix = 2;
      static QxtCurrency epsilon() throw()
      { return 0; }
      static QxtCurrency round_error() throw()
      { return 0; }

      static const int min_exponent = 0;
      static const int min_exponent10 = 0;
      static const int max_exponent = 0;
      static const int max_exponent10 = 0;

      static const bool has_infinity = false;
      static const bool has_quiet_NaN = false;
      static const bool has_signaling_NaN = false;
      static const float_denorm_style has_denorm = denorm_absent;
      static const bool has_denorm_loss = false;

      static QxtCurrency infinity() throw()
      { return static_cast<QxtCurrency>(0); }
      static QxtCurrency quiet_NaN() throw()
      { return static_cast<QxtCurrency>(0); }
      static QxtCurrency signaling_NaN() throw()
      { return static_cast<QxtCurrency>(0); }
      static QxtCurrency denorm_min() throw()
      { return static_cast<QxtCurrency>(0); }

      static const bool is_iec559 = false;
      static const bool is_bounded = true;
      static const bool is_modulo = true;

      static const bool traps = numeric_limits<qlonglong>::traps;
      static const bool tinyness_before = false;
      static const float_round_style round_style = round_toward_zero;
    };
}

#endif // QXTCURRENCY_H
