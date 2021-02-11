/**
 * IotWebConfTParameter.h -- IotWebConf is an ESP8266/ESP32
 *   non blocking WiFi/AP web configuration library for Arduino.
 *   https://github.com/prampec/IotWebConf
 *
 * Copyright (C) 2021 Balazs Kelemen <prampec+arduino@gmail.com>
 *                    rovo89
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#ifndef IotWebConfTParameterBuilder_h
#define IotWebConfTParameterBuilder_h

#include <IotWebConfTParameter.h>

namespace iotwebconf
{

template <typename ParamType> class Builder;

template <typename ParamType>
class AbstractBuilder
{
public:
  AbstractBuilder(const char* id) : _id(id) { };
  virtual ParamType build() const
  {
    return std::move(
      ParamType(this->_id, this->_label, this->_defaultValue));
  }

  Builder<ParamType>& label(const char* label)
    { this->_label = label; return static_cast<Builder<ParamType>&>(*this); }
  Builder<ParamType>& defaultValue(typename ParamType::DefaultValueType defaultValue)
    { this->_defaultValue = defaultValue; return static_cast<Builder<ParamType>&>(*this); }

protected:
  const char* _label;
  const char* _id;
  typename ParamType::DefaultValueType _defaultValue;
};

///////////////////////////////////////////////////////////////////////////

template <typename ValueType, int base>
class Builder<NumericTParameter<ValueType, base>> :
  public AbstractBuilder<NumericTParameter<ValueType, base>>
{
public:
  Builder<NumericTParameter<ValueType, base>>(const char* id) :
    AbstractBuilder<NumericTParameter<ValueType, base>>(id) { };
  virtual NumericTParameter<ValueType, base> build() const override
  {
    NumericTParameter<ValueType, base> instance =
      std::move(NumericTParameter<ValueType, base>(
        this->_label, this->_id, this->_defaultValue));
    this->apply(&instance);
    return instance;
  }
  Builder& min(ValueType min) { this->_minDefined = true; this->_min = min; return *this; }
  Builder& max(ValueType max) { this->_maxDefined = true; this->_max = max; return *this; }

protected:
  virtual NumericTParameter<ValueType, base>* apply(
    NumericTParameter<ValueType, base>* instance) const
  {
    if (this->_minDefined)
    {
      instance->setMin(this->_min);
    }
    if (this->_maxDefined)
    {
      instance->setMax(this->_max);
    }
    return instance;
  }

  bool _minDefined = false;
  bool _maxDefined = false;
  ValueType _min;
  ValueType _max;
};

template <typename ParamType>
class Builder : public AbstractBuilder<ParamType>
{
public:
  Builder(const char* id) : AbstractBuilder<ParamType>(id) { };
};

} // End namespace

#endif
