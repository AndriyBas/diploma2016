from getopt import getopt
from os import listdir, path, makedirs, remove
from re import sub
from subprocess import check_call
from shutil import rmtree, copytree
from time import sleep
from sys import argv

TRAIN_DIR = "../Pictures/"
IMAGE_POS_DIR = TRAIN_DIR+"/positive-clean"
IMAGE_POS_DIR_CROPPED = TRAIN_DIR+"/positive-clean-cropped"
MAIN_DIR = "data"
NEGATIVE_IMAGE_DIR = MAIN_DIR+"/negative-tutorial-haartraining"
SAMPLE_IMAGES_DIR = MAIN_DIR+"/positive-clean-cropped-samples"
HAAR_MAIN_DIR = MAIN_DIR+"/clean-cropped-haardata"

NEGATIVE_COLLECTION_PATH = NEGATIVE_IMAGE_DIR+"/"+NEGATIVE_IMAGE_DIR.split("/")[-1]+".txt"
SAMPLE_COLLECTION_PATH = SAMPLE_IMAGES_DIR+"/"+SAMPLE_IMAGES_DIR.split("/")[-1]+".txt"
VEC_SAMPLE_PATH = SAMPLE_IMAGES_DIR+".vec"

if __name__=="__main__":
    (createSamples, createVec, trainCascade) = (False, False, False)
    opts, args = getopt(argv[1:],"svt",["createSamples","createVec","trainCascade"])
    for opt, arg in opts:
        if(opt in ("--createSamples","-s")):
            createSamples = True
        elif(opt in ("--createVec","-v")):
            createVec = True
        elif(opt in ("--trainCascade","-t")):
            trainCascade = True

    # переконуємось що директорія виводу існує
    if(not path.isdir(SAMPLE_IMAGES_DIR)):
        makedirs(SAMPLE_IMAGES_DIR)

    # стоворюємо список негативних зображень
    negativeCollectionFile = open(NEGATIVE_COLLECTION_PATH, "w")
    negativeImagesNames = [f for f in listdir(NEGATIVE_IMAGE_DIR) if path.isfile(path.join(NEGATIVE_IMAGE_DIR,f)) and f.lower().endswith("jpg")]
    for f in negativeImagesNames:
        negativeCollectionFile.write(f)
        negativeCollectionFile.write("\n")
    negativeCollectionFile.close()

    # створюємо список позитивних зображень
    positiveImagesNames = [f for f in listdir(IMAGE_POS_DIR_CROPPED) if path.isfile(path.join(IMAGE_POS_DIR_CROPPED,f))]

    # createSamples: створення списку зображень-зразків
    # із зображеннь з папки IMAGE_POS_DIR_CROPPED, накладених поверх негативних зображень,
    if(createSamples):
        # для кожного зображення запускаємо команду create
        for f in positiveImagesNames:
            check_call(["opencv_createsamples",
                "-img", path.join(IMAGE_POS_DIR_CROPPED,f),
                "-bg", NEGATIVE_COLLECTION_PATH,
                "-info", path.join(SAMPLE_IMAGES_DIR,sub("(?i)jpg","txt",f)),
                "-num", "128",
                "-maxxangle", "0.0",
                "-maxyangle", "0.0",
                "-maxzangle", "0.3",
                "-bgcolor", "255",
                "-bgthresh", "8",
                "-w", "48",
                "-h", "48"])
            sleep(1)

    # createVec: створення файлу .vec із
    # позитивних зображень
    # якщо немає позитивних зображень,
    # то пошук відбувається у навчальній директорії
    if(createVec):
        positiveImagesCollectionsNames = [f for f in listdir(SAMPLE_IMAGES_DIR) if path.isfile(path.join(SAMPLE_IMAGES_DIR,f)) and f.lower().endswith("txt")]

        # якщо немає файлів колекції у директорії
        # та не було створено колекції до того
        # скопіювати чисті зразкові зображення у директорію навчання
        if((not positiveImagesCollectionsNames) and (not path.isfile(SAMPLE_COLLECTION_PATH))):
            rmtree(SAMPLE_IMAGES_DIR)
            copytree(IMAGE_POS_DIR, SAMPLE_IMAGES_DIR)
            positiveImagesCollectionsNames = [f for f in listdir(SAMPLE_IMAGES_DIR) if path.isfile(path.join(SAMPLE_IMAGES_DIR,f)) and f.lower().endswith("txt")]

        # якщо немає файлів колекції у директорії - створити її
        #
        # це еквівалент до:
        #     cd data/positive-clean-cropped-samples
        #     cat *.txt > positive-clean-cropped-samples.txt
        #     find . -name '*.txt' \! -name positive-clean-cropped-samples.txt -delete
        if(not path.isfile(SAMPLE_COLLECTION_PATH)):
            posImageCollectionFile = open(SAMPLE_COLLECTION_PATH, "w")
            for f in positiveImagesCollectionsNames:
                lines = open(path.join(SAMPLE_IMAGES_DIR,f), "r")
                for l in lines:
                    posImageCollectionFile.write(l)
                lines.close()
                remove(path.join(SAMPLE_IMAGES_DIR,f))
            posImageCollectionFile.close()

        check_call(["opencv_createsamples",
            "-info", SAMPLE_COLLECTION_PATH,
            "-vec", VEC_SAMPLE_PATH,
            "-bg", NEGATIVE_COLLECTION_PATH,
            "-num", str(sum(1 for line in open(SAMPLE_COLLECTION_PATH))),
            "-w", "40",
            "-h", "40"])
        sleep(1)

    # trainCascade: навчання каскаду
    if(trainCascade):
        if(not path.isdir(HAAR_MAIN_DIR)):
            makedirs(HAAR_MAIN_DIR)

        check_call(["opencv_traincascade",
            "-data", HAAR_MAIN_DIR,
            "-vec", VEC_SAMPLE_PATH,
            "-bg", NEGATIVE_COLLECTION_PATH,
            "-numPos", str(min(600, sum(1 for line in open(SAMPLE_COLLECTION_PATH)))),
            "-numNeg", str(min(100, sum(1 for line in open(NEGATIVE_COLLECTION_PATH)))),
            "-numStages", "20",
            "-precalcValBufSize", "768",
            "-precalcIdxBufSize", "768",
            "-featureType", "HAAR",
            "-w", "40",
            "-h", "40",
            "-minHitRate", "0.95",
            "-maxFalseAlarmRate", "0.6"])
        sleep(1)
