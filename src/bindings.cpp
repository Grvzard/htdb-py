#include <pybind11/pybind11.h>
#include "htdb.h"
#include <iostream>
#include <stdio.h>

namespace py = pybind11;


class Htdb {
public:
    Htdb() {
        _db = xdbNew(0, 0);
    }
    ~Htdb() {
        xdbFree(_db);
    }

    size_t getSize() const {
        return xdbSize(_db);
    }

    auto set(py::object key_, py::object value_) -> void {
        if (py::isinstance<py::int_>(key_)) {
            uint64_t key = key_.cast<uint64_t>();

            if (py::isinstance<py::bytes>(value_) || py::isinstance<py::str>(value_)) {
                std::string value = value_.cast<std::string>();
                _xdbSetIntBytes(_db, key, value.c_str(), static_cast<uint8_t>(value.length()));
            } else if (py::isinstance<py::int_>(value_)) {
                uint64_t value = value_.cast<uint64_t>();
                _xdbSetIntInt(_db, key, value);
            } else {
                throw py::type_error("invalid value type");
            }

        } else if (py::isinstance<py::bytes>(key_) || py::isinstance<py::str>(key_)) {
            std::string key = key_.cast<std::string>();

            if (py::isinstance<py::bytes>(value_) || py::isinstance<py::str>(value_)) {
                std::string value = value_.cast<std::string>();
                _xdbSetBytesBytes(_db, key.c_str(), static_cast<uint8_t>(key.length()), value.c_str(), static_cast<uint8_t>(value.length()));
            } else {
                throw py::type_error("invalid value type");
            }

        } else {
            throw py::type_error("key must be int, bytes or str");
        }
    }

    auto get(py::object key_) -> py::object {
        if (py::isinstance<py::int_>(key_)) {
            uint64_t key = key_.cast<uint64_t>();

            xobj *obj = xdbGetByInt(_db, key);

            if (obj == NULL) {
                throw py::key_error("Key not found");
            }

            if (obj->type == XOBJ_TYPE_INT) {
                uint64_t value = *(uint64_t *)obj->data;
                return py::cast(value);
            } else if (obj->type == XOBJ_TYPE_BYTES) {
                std::string value = std::string((char *)obj->data, obj->len);
                return py::bytes(value);
            } else {
                throw py::key_error("err: unknown value type");
            }

        } else if (py::isinstance<py::bytes>(key_) || py::isinstance<py::str>(key_)) {
            std::string key = key_.cast<std::string>();

            xobj *obj = xdbGetByBytes(_db, key.c_str(), static_cast<uint8_t>(key.length()));

            if (obj == NULL) {
                throw py::key_error("Key not found");
            }

            if (obj->type == XOBJ_TYPE_INT) {
                uint64_t value = *(uint64_t *)obj->data;
                return py::cast(value);
            } else if (obj->type == XOBJ_TYPE_BYTES) {
                std::string value = std::string((char *)obj->data, obj->len);
                return py::bytes(value);
            } else {
                throw std::runtime_error("err: unknown value type");
            }

        } else {
            throw py::type_error("invalid key type");
        }
    }
private:
    xdb *_db;
};

PYBIND11_MODULE(htdb, m) {
    py::class_<Htdb>(m, "Htdb")
        .def(py::init<>())
        .def("set", &Htdb::set)
        .def("get", &Htdb::get)
        .def("__len__",
            [](const Htdb &db) { return db.getSize(); },
            "getSize"
        )
    ;
}
