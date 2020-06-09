// //hiredis/hiredis.h
// /* Context for a connection to Redis */
// typedef struct redisContext {
//     int err; /* Error flags, 0 when there is no error */
//     char errstr[128]; /* String representation of error when applicable */
//     int fd; 
//     int flags;
//     char *obuf; /* Write buffer */
//     redisReader *reader; /* Protocol reader */
// } redisContext;
 
// /* This is the reply object returned by redisCommand() */
// #define REDIS_REPLY_STRING 1
// #define REDIS_REPLY_ARRAY 2
// #define REDIS_REPLY_INTEGER 3
// #define REDIS_REPLY_NIL 4
// #define REDIS_REPLY_STATUS 5
// #define REDIS_REPLY_ERROR 6
// typedef struct redisReply {
//     int type; /* REDIS_REPLY_* */
//     long long integer; /* The integer when type is REDIS_REPLY_INTEGER */
//     int len; /* Length of string */
//     char *str; /* Used for both REDIS_REPLY_ERROR and REDIS_REPLY_STRING */
//     size_t elements; /* number of elements, for REDIS_REPLY_ARRAY */
//     struct redisReply **element; /* elements vector for REDIS_REPLY_ARRAY */
// } redisReply;
 
// redisContext *redisConnectWithTimeout(const char *ip, int port, struct timeval tv);
// void redisFree(redisContext *c);