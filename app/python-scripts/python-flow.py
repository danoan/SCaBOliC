#!/usr/bin/python3

import subprocess,sys,os

PROJECT_FOLDER=None

def clean_folder(outputFolder):
    subprocess.call( ["mogrify -format pgm %s/*.png" % (outputFolder,)],shell=True )
    subprocess.call( ["rm %s/*.png" % (outputFolder,)],shell=True)

def generate_shape(di):
    bin = "%s/%s" % (PROJECT_FOLDER,"cmake-build-debug/app/generate-shapes/generate-shapes")
    subprocess.call( [bin,
                      di["outputPath"],
                      di["shape"],
                      di["gridStep"],
                      ])

def python_writer(di):
    bin = "%s/%s" % (PROJECT_FOLDER,"cmake-build-debug/app/python-writer/python-writer")
    subprocess.call( [bin,
                      "%s%s" % ("-r",di["r"]),
                      "%s%s" % ("-q",di["q"]),
                      "%s%s" % ("-g",di["g"]),
                      "%s%s" % ("-S",di["S"]),
                      "%s%s" % ("-h",di["h"]),
                      "%s%s" % ("-O",di["O"]),
                      "%s%s" % ("-f","around-contour"),
                      "%s%s" % ("-l","-1"),
                      "-d",
                      "%s%s" % ("",di["outputPath"])
                      ])

def python_qpsolver(di):
    pythonSource = "%s/%s" % (PROJECT_FOLDER,"app/python-scripts/python-qpsolver.py")
    subprocess.call( ["python3",
                      pythonSource,
                      di["modelFilepath"],
                      di["outputPath"],
                      di["invert"]
                      ])

def main():
    if len(sys.argv)<7:
        print("Usage: projectFolder shapeStr gridStep iterations outputFolder invertSolution(yes/no)")
        exit(1)

    global PROJECT_FOLDER

    PROJECT_FOLDER = sys.argv[1]
    shapeStr = sys.argv[2]
    gridStep = sys.argv[3]
    iterations = int(sys.argv[4])
    outputFolder = sys.argv[5]
    invertSolution = sys.argv[6]

    diGS = {"outputPath":"%s/%s" % (outputFolder,"0.pgm"),
            "shape":shapeStr,
            "gridStep":gridStep}

    generate_shape(diGS)

    diPW = {"r":"5",
          "q":"1.0",
          "g":"0.0",
          "S":shapeStr,
          "h":gridStep,
          "O":"convex",
          "outputPath": ""}

    for i in range(iterations):
        diPW["S"] = "%s/%d.pgm" % (outputFolder,i)
        diPW["outputPath"] = "%s/models/model_%d.py" % (outputFolder,i)
        diPW["O"] = "concave" if i%2==0 else "convex"

        python_writer(diPW)

        diQPS={"modelFilepath":diPW["outputPath"],
               "outputPath":"%s/%d.png" % (outputFolder,i+1),
               "invert":invertSolution,
               }

        diQPS["invert"] = "no" if i%2==0 else "yes"

        python_qpsolver(diQPS)
        clean_folder(outputFolder)



main()