# Arguments
# commandArgs -> gets arguments from the command line 
# as.numeric -> converts string to numeric value

print("usage: Rscript plot.R output_from_cmeasure 50")

args = commandArgs();
args
inpath <- args[6];
skipLen <- as.numeric(args[7]);

# c -> Input
dbs <- c();
maxX <- 0;
maxY <- 0;

# Input of data 

# Get the file 
#filename <- paste(c(inpath, ".txt"), collapse="")
filename <- inpath
pdfname <- paste(c(inpath, ".pdf"), collapse="")
	
# Put data on a table
db <- read.table(filename);
names(db) <- c("time", "power");
db <- db[seq(1, length(db$time), skipLen),];

# dbs <- c(dbs, db);
maxX <- max(c(maxX, db$time));
maxY <- max(c(maxY, db$power));

# Set or query graphical parameters 
numTests = 5 # eu fiz
par(mfrow=c(numTests, 1), mar=c(2, 5, 1, 1), oma=c(3, 0, 0, 0));


# Put data on a table
db <- read.table(filename);
names(db) <- c("time", "power");
db <- db[seq(1, length(db$time), skipLen),];

# Plot graphs 
# Get data from table 
# Labels for each value (X, Y)
pdf(pdfname, width=10, height=3)
par(mar=c(4,4,2,2))

plot(db$time, db$power, pch="",
	xlim=c(0, maxX), ylim=c(0, maxY),
	xlab="Time (s)", ylab="iPower (W)");
lines(db$time, db$power);
