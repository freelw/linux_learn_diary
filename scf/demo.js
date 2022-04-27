/**************************************************
Node8.9_OperateRedis
Reference:
1、https://www.npmjs.com/package/ioredis - redis interfaces
***************************************************/

const redis = require("ioredis");
const redisStore = new redis({
    port: 6379, // Redis instance port, redis实例端口
    host: "10.0.2.3", // Redis instance host, redis实例host
    family: 4,
    password: "mJmmXvtzbwF4Vr3R3KRP", // Redis instance password, redis实例密码
    db: 0
});
exports.main_handler = async (event, context, callback) => {

    redisStore.set('foo', 'bar22' + Date.now().toString());
    const promise = new Promise((resolve, reject) => {
        redisStore.get('foo', function (err, result) {
            resolve({
                "isBase64Encoded": false,
                "statusCode": 200,
                "headers": { "Content-Type": "text/html" },
                "body": `<html><body><h1>Heading</h1><p>Paragraph.${result}</p></body></html>`,
            })
        })
    })

    return promise
};
