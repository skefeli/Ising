#bsub -q 1nd isingRun.sh
cp /afs/cern.ch/user/s/skefeli/public/554/a.out .
cp /afs/cern.ch/user/s/skefeli/public/554/clearfile.dat .
cp /afs/cern.ch/user/s/skefeli/public/554/datafile.dat .
cp /afs/cern.ch/user/s/skefeli/public/554/run.sh .

./run.sh
cp datafile.dat /afs/cern.ch/user/s/skefeli/public/554/
cp moresweep.dat /afs/cern.ch/user/s/skefeli/public/554/
