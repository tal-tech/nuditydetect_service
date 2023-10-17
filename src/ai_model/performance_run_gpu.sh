export LD_LIBRARY_PATH=./lib

# 模型文件路径
MODEL_PATH=model/cls_image_nudity_v1.0.0.trt

# 配置文件路径
#CONFIG_PATH=model/config.ini
CONFIG_PATH=/
# 最大batch
MAX_BATCH=1

# 输入图片路径
IMAGE_DIR=/home/atm/workspace/nuditydetect_service/src/ai_model/images/testing

# 图片中人脸框信息路径
IMAGE_INFO_PATH=testing_images.txt

# 循环次数(-1表示无限循环)
RUN_LOOP=1

./bin/performance_testing $MODEL_PATH $CONFIG_PATH $MAX_BATCH $IMAGE_DIR $IMAGE_INFO_PATH $RUN_LOOP
