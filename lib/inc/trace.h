#pragma once

#include <memory>
#include <fstream>

using namespace std;

class tracer
{
public :
    tracer()
    {
        _enabled = false;

#ifdef _DEBUG
        // TRACE always on in debug
        _enabled = true;
#else
        if (std::getenv("ENABLE_TRACE"))
            _enabled = true;
        else
            _enabled = false;
#endif
        _stream = std::make_unique<ofstream>();
    }

    void init(const char *filename)
    {
        _file_name = filename;
        _stream->open(_file_name);
    }

    ~tracer()
    {
        _stream->close();
    }

    bool enabled() { return _enabled; }

    void trace(string str)
    {
        if (_stream)
            _stream->write(str.c_str(), str.size());
    }

    static tracer &get()
    {
        static tracer s_trace;
        return s_trace;
    }

private :
    string _service_name;
    string _file_name;
    bool _enabled;
    unique_ptr<ofstream> _stream;
};

#define INIT_TRACE(filename) tracer::get().init(filename);

// Always on - should only used for non-performance critical scenarios
// No need to flush - endl automatically flushes  
#define LOG(expr) tracer::get().trace(expr);

// Optionally enabled in release and always on in debug
#define TRACE(expr) if (tracer.get().enabled()) { LOG(expr); } 

// Debug-only trace
#ifdef _DEBUG
#define DIAG(expr) if (tracer.get().enabled()) { LOG(expr); } 
#else
#define DIAG(expr) ;
#endif