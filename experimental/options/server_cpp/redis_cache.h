// Copyright (c) Mike Kipnis
// ============================================================
// Redis Wrapper
// ============================================================
#pragma once

namespace redis_client {

class RedisClient {
    
public:
    RedisClient( const std::string& host = "127.0.0.1", int port = 6379)
    {
        context_ = redisConnect(host.c_str(), port);
        
        if (!context_ || context_->err) {
            
            std::string err =
            context_ ? context_->errstr
            : "Cannot allocate redis context";
            
            throw std::runtime_error(err);
        }
    }
    
    ~RedisClient() {
        
        if (context_) {
            redisFree(context_);
        }
    }
    
    void set(const std::string& key, const std::string& value, int ttl_seconds = 3600)
    {
        
        redisReply* reply = (redisReply*)redisCommand(
                                                      context_,
                                                      "SETEX %s %d %s",
                                                      key.c_str(),
                                                      ttl_seconds,
                                                      value.c_str());
        
        if (!reply) {
            throw std::runtime_error("Redis SETEX failed");
        }
        
        freeReplyObject(reply);
    }
    
    std::optional<std::string> get(const std::string& key)
    {
        
        redisReply* reply = (redisReply*)redisCommand(
                                                      context_,
                                                      "GET %s",
                                                      key.c_str());
        
        if (!reply) {
            throw std::runtime_error("Redis GET failed");
        }
        
        std::optional<std::string> result;
        
        if (reply->type == REDIS_REPLY_STRING) {
            result = std::string(reply->str);
        }
        
        freeReplyObject(reply);
        
        return result;
    }
    
private:
    redisContext* context_;
};

inline RedisClient& get_thread_redis()
{
    const char* env_host = std::getenv("REDIS_HOST");
    
    std::string redis_host = env_host ? env_host : "localhost";
    
    thread_local RedisClient redis(redis_host);
    
    return redis;
}

};

