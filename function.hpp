//   Copyright 2018 SiLeader and Cerussite.
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#ifndef FUNCTION_HPP
#define FUNCTION_HPP
#include <utility>
#include <stdexcept>

namespace func {
    class bad_function_call : std::exception {
        using exception::exception;
    };

    template <class> class function;

    template <class Ret, class... Args> class function<Ret(Args...)> {
    private:
        struct _function_holder_base {
            _function_holder_base() = default;
            _function_holder_base(const _function_holder_base&) = default;
            _function_holder_base(_function_holder_base&&) = default;
            _function_holder_base&
            operator=(const _function_holder_base&) = default;
            _function_holder_base& operator=(_function_holder_base&&) = default;

            virtual ~_function_holder_base() = default;

            virtual Ret call(Args... args) const = 0;
            virtual _function_holder_base* new_instance() const = 0;
        };
        class _function_pointer_holder : public _function_holder_base {
        private:
            using function_type = Ret (*)(Args...);

            function_type _function;

        public:
            using _function_holder_base::_function_holder_base;

            _function_pointer_holder(function_type f) : _function(f) {
            }

            Ret call(Args... args) const override {
                return _function(std::forward<Args>(args)...);
            }
            _function_holder_base* new_instance() const override {
                return new _function_pointer_holder(_function);
            }
        };
        template <class T>
        class _function_object_holder : public _function_holder_base {
        private:
            T _object;

        public:
            using _function_holder_base::_function_holder_base;

            _function_object_holder(T o) : _object(o) {
            }

            Ret call(Args... args) const override {
                return _object(std::forward<Args>(args)...);
            }
            _function_holder_base* new_instance() const override {
                return new _function_object_holder<T>(_object);
            }
        };

    private:
        _function_holder_base* _holder;

    private:
        void _clear() {
            delete _holder;
            _holder = nullptr;
        }

    public:
        function() : _holder(nullptr) {
        }
        function(std::nullptr_t) : function() {
        }

        function(Ret (*fp)(Args...))
                : _holder(new _function_pointer_holder(fp)) {
        }
        template <class T>
        function(T&& f)
                : _holder(new _function_object_holder<T>(std::forward<T>(f))) {
        }

        function& operator=(Ret (*fp)(Args...)) {
            _clear();
            _holder = new _function_pointer_holder(fp);
            return *this;
        }
        template <class T> function& operator=(T&& f) {
            _clear();
            _holder = new _function_object_holder<T>(std::forward<T>(f));
            return *this;
        }

        function(const function& rhs) : function() {
            if (rhs) _holder = rhs._holder->new_instance();
        }
        function(function&& rhs) : function(rhs._holder) {
            rhs._holder = nullptr;
        }

        function& operator=(const function& rhs) {
            if (rhs) _holder = rhs._holder->new_instance();
            return *this;
        }
        function& operator=(function&& rhs) {
            _holder = rhs._holder;
            rhs._holder = nullptr;
            return *this;
        }

        function& operator=(std::nullptr_t) {
            _clear();
            return *this;
        }

    public:
        explicit operator bool() const noexcept {
            return _holder != nullptr;
        }

    public:
        Ret operator()(Args... args) const {
            if(*this)return _holder->call(std::forward<Args>(args)...);
            throw bad_function_call();
        }
    };
} // namespace func

#endif // FUNCTION_HPP
