//
// Created by rcohen on 20/6/2021.
//
#pragma once
#ifndef LIBXLA_XLA_BACKEND_H
#define LIBXLA_XLA_BACKEND_H
#include <string>
#include <memory>
#include <condition_variable>

#include "xla_stream.h"
#include "xla_commands.h"
#include "xla_operator.h"

namespace libxla {
    class Backend {
        public:
        virtual ~Backend() = default;

        virtual StreamPtr createStream() = 0;
        virtual void wait_for_all() = 0;

        virtual std::shared_ptr<Signal> createSignal() = 0;

        virtual ComputeCmdPtr createComputeCmd(const std::string &op_name, const TensorsList &inputs, const TensorsList &outputs, const AttrList &attributes) = 0;
        virtual CommandPtr createTestCmd(int *variable, int test_val, int sleep_ms=0) = 0;

        virtual ComputeCmdPtr AddCmd(const Tensor &lhs, const Tensor &rhs, const Tensor &output) = 0;
    };

    class BackendManager {
        public:
            typedef enum {
                CPU = 0,
                GAUDI = 1,

                NO_BACKENDS = GAUDI+1
            } BACKEND_TYPE;
            static BackendManager &Inst();            
            void set_backend(BACKEND_TYPE type);
            inline Backend *backend() const {return _active_backend;}

    private:
        Backend *_active_backend;
        Backend *_backends[NO_BACKENDS];
        BackendManager();
    };

}

#endif // LIBXLA_XLA_BACKEND_H