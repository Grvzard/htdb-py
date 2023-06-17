#include <pybind11/pybind11.h>
#include "htdb.h"
#include <iostream>
#include <stdio.h>

namespace py = pybind11;
using namespace pybind11::literals;


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
                _xdbSetIntBytes(_db, key, value.c_str(), static_cast<xobjlen_t>(value.length()));
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
                _xdbSetBytesBytes(_db, key.c_str(), static_cast<xobjlen_t>(key.length()), value.c_str(), static_cast<xobjlen_t>(value.length()));
            } else {
                throw py::type_error("invalid value type");
            }

        } else {
            throw py::type_error("key must be int, bytes or str");
        }
    }

    auto get(py::object key_) -> py::object {
        xobj *valobj = NULL;

        if (py::isinstance<py::int_>(key_)) {
            uint64_t key = key_.cast<uint64_t>();
            valobj = xdbGetByInt(_db, key);

        } else if (py::isinstance<py::bytes>(key_) || py::isinstance<py::str>(key_)) {
            std::string key = key_.cast<std::string>();
            valobj = xdbGetByBytes(_db, key.c_str(), static_cast<xobjlen_t>(key.length()));

        } else {
            throw py::type_error("invalid key type");
        }

        if (valobj == NULL) {
            throw py::key_error("Key not found");
        }

        if (valobj->type == XOBJ_TYPE_INT) {
            uint64_t value = *(uint64_t *)valobj->data;
            return py::cast(value);
        } else if (valobj->type == XOBJ_TYPE_BYTES) {
            std::string value = std::string((char *)valobj->data, valobj->len);
            return py::bytes(value);
        } else {
            throw std::runtime_error("err: unknown value type");
        }
    }

    auto has(py::object key_) -> bool {
        if (py::isinstance<py::int_>(key_)) {
            uint64_t key = key_.cast<uint64_t>();
            bool ret = static_cast<bool>(xdbHasInt(_db, key));
            return ret;

        } else if (py::isinstance<py::bytes>(key_) || py::isinstance<py::str>(key_)) {
            std::string key = key_.cast<std::string>();
            bool ret = static_cast<bool>(xdbHasBytes(_db, key.c_str(), static_cast<xobjlen_t>(key.length())));
            return ret;

        } else {
            throw py::type_error("invalid key type");
        }
    }

    auto remove(py::object key_) -> void {
        if (py::isinstance<py::int_>(key_)) {
            uint64_t key = key_.cast<uint64_t>();
            xdbDelInt(_db, key);

        } else if (py::isinstance<py::bytes>(key_) || py::isinstance<py::str>(key_)) {
            std::string key = key_.cast<std::string>();
            xdbDelBytes(_db, key.c_str(), static_cast<xobjlen_t>(key.length()));

        } else {
            throw py::type_error("invalid key type");
        }
    }

    auto dump(std::string filename) -> void {
        if (filename.empty()) {
            xdbDump(_db, stdout);
            std::cout << std::endl;
            return;
        }
        FILE *fp = fopen(filename.c_str(), "wb");
        xdbDump(_db, fp);
        fclose(fp);
    }

    auto load(std::string filename) -> void {
        FILE *fp = fopen(filename.c_str(), "rb");
        xdbLoad(_db, fp);
        fclose(fp);
    }
private:
    xdb *_db;
};

PYBIND11_MODULE(htdb, m) {
    py::class_<Htdb>(m, "Htdb")
        .def(py::init<>())
        .def("set", &Htdb::set)
        .def("get", &Htdb::get)
        .def("has", &Htdb::has)
        .def("remove", &Htdb::remove)
        .def("dump", &Htdb::dump, "filename"_a = std::string())
        .def("load", &Htdb::load, "filename"_a)
        .def("__len__", &Htdb::getSize)
        .def("__contains__", &Htdb::has)
    ;
}
