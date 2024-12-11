# spark-sql 写 iceberg

## qcloud cos

```
bin/spark-sql --packages org.apache.iceberg:iceberg-spark-runtime-3.5_2.12:1.7.0,com.qcloud.cos:hadoop-cos:8.2.7,com.qcloud:cos_api-bundle:5.6.228 \
     --conf "spark.hadoop.fs.defaultFS=cosn://cicd-1312904344" \
    --conf "spark.hadoop.fs.cosn.userinfo.secretId=xxxxxx" \
    --conf "spark.hadoop.fs.cosn.userinfo.secretKey=xxxxxx" \
    --conf "spark.hadoop.fs.cosn.bucket.region=ap-shanghai" \
    --conf "spark.hadoop.fs.cosn.impl=org.apache.hadoop.fs.CosFileSystem" \
    --conf "spark.hadoop.fs.AbstractFileSystem.cosn.impl=org.apache.hadoop.fs.CosN" \
    --conf spark.sql.catalog.local=org.apache.iceberg.spark.SparkCatalog \
    --conf spark.sql.catalog.local.type=hadoop \
   --conf spark.sql.catalog.local.warehouse=/dawang/dawang_warehouse
```

## ks3

```
bin/spark-sql --packages org.apache.iceberg:iceberg-spark-runtime-3.5_2.12:1.7.0,org.apache.hadoop:hadoop-aws:3.3.4,com.amazonaws:aws-java-sdk-bundle:1.12.262 \
    --conf "spark.hadoop.fs.s3a.access.key=xxxxxx" \
    --conf "spark.hadoop.fs.s3a.secret.key=xxxxxx" \
    --conf "spark.hadoop.fs.s3a.endpoint=ks3-cn-beijing.ksyuncs.com" \
    --conf spark.sql.catalog.local=org.apache.iceberg.spark.SparkCatalog \
    --conf spark.sql.catalog.local.type=hadoop \
   --conf spark.sql.catalog.local.warehouse=s3a://commontestdata/dawang_warehouse
```

## sql
```
use local;

CREATE TABLE delete_mdvmb_base (
  n_nationkey INT,
  n_name STRING,
  n_regionkey INT,
  n_comment STRING)
USING iceberg
TBLPROPERTIES (
  'format' = 'iceberg/parquet',
  'write.delete.mode'='merge-on-read',
  'write.update.mode'='merge-on-read',
  'write.merge.mode'='merge-on-read',
  'format-version' = '2');

insert into delete_mdvmb_base values
(1, "a", 2, "c"),
(2, "a", 2, "c"),
(3, "a", 2, "c"),
(4, "a", 2, "c"),
(5, "a", 2, "c"),
(6, "a", 2, "c"),
(7, "a", 2, "c"),
(8, "a", 2, "c")
;
```
