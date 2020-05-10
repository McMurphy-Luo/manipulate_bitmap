#ifndef ZOOM_WINDOWS_SIGNALS_H_
#define ZOOM_WINDOWS_SIGNALS_H_

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <cassert>
#include <list>
#include <functional>
#include <algorithm>
#include <memory>

namespace signals
{
  namespace detail
  {
    struct connection_internal_base {
      virtual ~connection_internal_base() {

      }

      virtual void disconnect() = 0;
    };

    template<typename T>
    struct signal_detail;
    
    template<typename T>
    struct signal_shared_block {
      std::tr1::weak_ptr<signal_detail<T>> caller;
      std::tr1::weak_ptr<signal_detail<T>> callee;
    };

    template<typename T>
    struct slot_shared_block {
      std::tr1::function<T> the_function;
    };

    template<typename T>
    void Disconnect(std::tr1::shared_ptr<signal_shared_block<T>> shared_block);

    template<typename T>
    struct signal_detail {
      std::list<std::tr1::shared_ptr<slot_shared_block<T>>> connections;
      std::list<std::tr1::shared_ptr<signal_shared_block<T>>> signals_connected_to_me;
      std::list<std::tr1::shared_ptr<signal_shared_block<T>>> connected_signals;

      ~signal_detail() {
        
      }

      template<typename T1>
      void operator()(T1 param1) {
        std::list<std::tr1::shared_ptr<slot_shared_block<T>>>::iterator it_slot = connections.begin();
        for (; it_slot != connections.end(); ++it_slot) {
          (*it_slot)->the_function(param1);
        }
        std::list<std::tr1::shared_ptr<signal_shared_block<T>>>::iterator it_connected_signal = connected_signals.begin();
        for (; it_connected_signal != connected_signals.end(); ++it_connected_signal) {
          (*it_connected_signal)->callee.lock()->operator()(param1);
        }
      }

      template<typename T1, typename T2>
      void operator()(T1 param1, T2 param2) {
        std::list<std::tr1::shared_ptr<slot_shared_block<T>>>::iterator it_slot = connections.begin();
        for (; it_slot != connections.end(); ++it_slot) {
          (*it_slot)->the_function(param1, param2);
        }
        std::list<std::tr1::shared_ptr<signal_shared_block<T>>>::iterator it_connected_signal = connected_signals.begin();
        for (; it_connected_signal != connected_signals.end(); ++it_connected_signal) {
          (*it_connected_signal)->callee.lock()->operator()<T1, T2>(param1, param2);
        }
      }

      template<typename T1, typename T2, typename T3>
      void operator()(T1 param1, T2 param2, T3 param3) {
        std::list<std::tr1::shared_ptr<slot_shared_block<T>>>::iterator it_slot = connections.begin();
        for (; it_slot != connections.end(); ++it_slot) {
          (*it_slot)->the_function(param1, param2, param3);
        }
        std::list<std::tr1::shared_ptr<signal_shared_block<T>>>::iterator it_connected_signal = connected_signals.begin();
        for (; it_connected_signal != connected_signals.end(); ++it_connected_signal) {
          (*it_connected_signal)->callee.lock()->operator()(param1, param2, param3);
        }
      }

      template<typename T1, typename T2, typename T3, typename T4>
      void operator()(T1 param1, T2 param2, T3 param3, T4 param4) {
        std::list<std::tr1::shared_ptr<slot_shared_block<T>>>::iterator it_slot = connections.begin();
        for (; it_slot != connections.end(); ++it_slot) {
          (*it_slot)->the_function(param1, param2, param3, param4);
        }
        std::list<std::tr1::shared_ptr<signal_shared_block<T>>>::iterator it_connected_signal = connected_signals.begin();
        for (; it_connected_signal != connected_signals.end(); ++it_connected_signal) {
          (*it_connected_signal)->callee.lock()->operator()(param1, param2, param3, param4);
        }
      }

      template<typename T1, typename T2, typename T3, typename T4, typename T5>
      void operator()(T1 param1, T2 param2, T3 param3, T4 param4, T5 param5) {
        std::list<std::tr1::shared_ptr<slot_shared_block<T>>>::iterator it_slot = connections.begin();
        for (; it_slot != connections.end(); ++it_slot) {
          (*it_slot)->the_function(param1, param2, param3, param4, param5);
        }
        std::list<std::tr1::shared_ptr<signal_shared_block<T>>>::iterator it_connected_signal = connected_signals.begin();
        for (; it_connected_signal != connected_signals.end(); ++it_connected_signal) {
          (*it_connected_signal)->callee.lock()->operator()(param1, param2, param3, param4, param5);
        }
      }

      template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
      void operator()(T1 param1, T2 param2, T3 param3, T4 param4, T5 param5, T6 param6) {
        std::list<std::tr1::shared_ptr<slot_shared_block<T>>>::iterator it_slot = connections.begin();
        for (; it_slot != connections.end(); ++it_slot) {
          (*it_slot)->the_function(param1, param2, param3, param4, param5, param6);
        }
        std::list<std::tr1::shared_ptr<signal_shared_block<T>>>::iterator it_connected_signal = connected_signals.begin();
        for (; it_connected_signal != connected_signals.end(); ++it_connected_signal) {
          (*it_connected_signal)->callee.lock()->operator()(param1, param2, param3, param4, param5, param6);
        }
      }

      template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
      void operator()(T1 param1, T2 param2, T3 param3, T4 param4, T5 param5, T6 param6, T7 param7) {
        std::list<std::tr1::shared_ptr<slot_shared_block<T>>>::iterator it_slot = connections.begin();
        for (; it_slot != connections.end(); ++it_slot) {
          (*it_slot)->the_function(param1, param2, param3, param4, param5, param6, param7);
        }
        std::list<std::tr1::shared_ptr<signal_shared_block<T>>>::iterator it_connected_signal = connected_signals.begin();
        for (; it_connected_signal != connected_signals.end(); ++it_connected_signal) {
          (*it_connected_signal)->callee.lock()->operator()(param1, param2, param3, param4, param5, param6, param7);
        }
      }

      template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
      void operator()(T1 param1, T2 param2, T3 param3, T4 param4, T5 param5, T6 param6, T7 param7, T8 param8) {
        std::list<std::tr1::shared_ptr<slot_shared_block<T>>>::iterator it_slot = connections.begin();
        for (; it_slot != connections.end(); ++it_slot) {
          (*it_slot)->the_function(param1, param2, param3, param4, param5, param6, param7, param8);
        }
        std::list<std::tr1::shared_ptr<signal_shared_block<T>>>::iterator it_connected_signal = connected_signals.begin();
        for (; it_connected_signal != connected_signals.end(); ++it_connected_signal) {
          (*it_connected_signal)->callee.lock()->operator()(param1, param2, param3, param4, param5, param6, param7, param8);
        }
      }
    };

    template<typename T>
    struct signal_slot_connection : public connection_internal_base {
      std::tr1::weak_ptr<signal_detail<T>> the_signal;
      std::tr1::shared_ptr<slot_shared_block<T>> the_shared_block;
      virtual ~signal_slot_connection() {
        disconnect();
      }

      virtual void disconnect() override {
        std::tr1::shared_ptr<signal_detail<T>> the_signal_locked = the_signal.lock();
        if (the_signal_locked) {
          std::list<std::tr1::shared_ptr<slot_shared_block<T>>>::const_iterator it = 
            std::find(the_signal_locked->connections.begin(), the_signal_locked->connections.end(), the_shared_block);
          if (it != the_signal_locked->connections.end()) {
            the_signal_locked->connections.erase(it);
          }
        }
        the_signal.reset();
        the_shared_block.reset();
      }
    };

    template<typename T>
    void Disconnect(std::tr1::shared_ptr<signal_shared_block<T>> shared_block) {
      std::tr1::shared_ptr<signal_detail<T>> caller = shared_block->caller.lock();
      std::tr1::shared_ptr<signal_detail<T>> callee = shared_block->callee.lock();
      std::list<std::tr1::shared_ptr<signal_shared_block<T>>>::iterator it_connected_signal = caller->connected_signals.begin();
      while (it_connected_signal != caller->connected_signals.end()) {
        if ((*it_connected_signal)->callee.lock() == callee) {
          it_connected_signal = caller->connected_signals.erase(it_connected_signal);
          break;
        } else {
          ++it_connected_signal;
        }
      }
      it_connected_signal = callee->signals_connected_to_me.begin();
      while (it_connected_signal != callee->signals_connected_to_me.end()) {
        if ((*it_connected_signal)->caller.lock() == caller) {
          it_connected_signal = callee->signals_connected_to_me.erase(it_connected_signal);
          break;
        } else {
          ++it_connected_signal;
        }
      }
    }

    template<typename T>
    struct signal_signal_connection : public connection_internal_base {
    public:
      signal_signal_connection(std::tr1::shared_ptr<signal_shared_block<T>> shared_block)
        : shared_block(shared_block) {

      }

      std::tr1::shared_ptr<signal_shared_block<T>> shared_block;
      virtual ~signal_signal_connection() {
        disconnect();
      }

      virtual void disconnect() override {
        if (!shared_block) {
          return;
        }
        if (!shared_block->caller.expired() && !shared_block->callee.expired()) {
          Disconnect(shared_block);
        }
        shared_block.reset();
      }
    };
  }

  class connection {
  public:
    connection(std::tr1::shared_ptr<detail::connection_internal_base> connection_internal_detail)
      : connection_detail_(connection_internal_detail) {

    }

    virtual ~connection() {

    }

    void disconnect() { connection_detail_->disconnect(); }

  private:
    std::tr1::shared_ptr<detail::connection_internal_base> connection_detail_;
  };

  template<typename T>
  class signal {
  public:
    signal()
      : signal_detail_(new detail::signal_detail<T>()) {

    }

    ~signal() {
      while (!signal_detail_->signals_connected_to_me.empty()) {
        detail::Disconnect<T>(*(signal_detail_->signals_connected_to_me.begin()));
      }
      while (!signal_detail_->connected_signals.empty()) {
        detail::Disconnect<T>(*(signal_detail_->connected_signals.begin()));
      }
    }

    connection connect(const std::tr1::function<T>& the_function) {
      std::tr1::shared_ptr<detail::signal_slot_connection<T>> connection_detail(new detail::signal_slot_connection<T>());
      std::tr1::shared_ptr<detail::slot_shared_block<T>> the_block(new detail::slot_shared_block<T>());
      the_block->the_function = the_function;
      connection_detail->the_shared_block = the_block;
      connection_detail->the_signal = signal_detail_;
      signal_detail_->connections.push_back(the_block);
      connection result(connection_detail);
      return result;
    }

    connection connect(const signal<T>& another) {
      std::tr1::shared_ptr<detail::signal_shared_block<T>> shared_block(new detail::signal_shared_block<T>);
      shared_block->callee = another.signal_detail_;
      shared_block->caller = signal_detail_;
      signal_detail_->connected_signals.push_back(shared_block);
      another.signal_detail_->signals_connected_to_me.push_back(shared_block);
      std::tr1::shared_ptr<detail::signal_signal_connection<T>> connection_concrete(new detail::signal_signal_connection<T>(shared_block));
      connection result(connection_concrete);
      return result;
    }

    template<typename T1>
    void operator()(T1 param1) {
      signal_detail_->operator()<T1>(param1);
    }

    template<typename T1, typename T2>
    void operator()(T1 param1, T2 param2) {
      signal_detail_->operator()<T1, T2>(param1, param2);
    }

    template<typename T1, typename T2, typename T3>
    void operator()(T1 param1, T2 param2, T3 param3) {
      signal_detail_->operator()(param1, param2, param3);
    }

    template<typename T1, typename T2, typename T3, typename T4>
    void operator()(T1 param1, T2 param2, T3 param3, T4 param4) {
      signal_detail_->operator(param1, param2, param3, param4);
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    void operator()(T1 param1, T2 param2, T3 param3, T4 param4, T5 param5) {
      signal_detail_->operator(param1, param2, param3, param4, param5);
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    void operator()(T1 param1, T2 param2, T3 param3, T4 param4, T5 param5, T6 param6) {
      signal_detail_->operator(param1, param2, param3, param4, param5, param6);
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    void operator()(T1 param1, T2 param2, T3 param3, T4 param4, T5 param5, T6 param6, T7 param7) {
      signal_detail_->operator(param1, param2, param3, param4, param5, param6, param7);
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    void operator()(T1 param1, T2 param2, T3 param3, T4 param4, T5 param5, T6 param6, T7 param7, T8 param8) {
      signal_detail_->operator(param1, param2, param3, param4, param5, param6, param7, param8);
    }

  private:
    signal(const signal&);
    signal& operator=(const signal&);

  private:
    std::tr1::shared_ptr<detail::signal_detail<T>> signal_detail_;
  };
}

#endif // ZOOM_WINDOWS_SIGNALS_H_