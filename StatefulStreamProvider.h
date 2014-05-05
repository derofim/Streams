#ifndef STATEFUL_STREAM_PROVIDER_H
#define STATEFUL_STREAM_PROVIDER_H

#include "StreamProvider.h"
#include "Utility.h"

#include <list>

template<typename T>
class StatefulStreamProvider : public StreamProvider<T> {

public:
    StatefulStreamProvider(StreamProviderPtr<T> source)
        : source_(std::move(source)) {}

    std::shared_ptr<T> get() override {
        return *current_;
    }

    bool advance() override {
        if(first_) {
            first_ = false;
            while(source_->advance()) {
                state_.push_back(source_->get());
            }
            current_ = state_.begin();
            return current_ != state_.end();
        }
        ++current_;
        return current_ != state_.end();
    }

private:
    using List = std::list<std::shared_ptr<T>>;
    using Iterator = typename List::iterator;

    bool first_ = true;
    StreamProviderPtr<T> source_;
    List state_;
    Iterator current_;
    Iterator end_;

};

#endif