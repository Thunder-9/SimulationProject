file1="simulation_file1.data"
file2="simulation_file2.data"
file3="simulation_file3.data"
data=read.table(file1)
attach(data);

lambda = V1
waitTime1 = V2
percentile1 = V3

detach(data)
data=read.table(file2)
attach(data)
waitTime2 = V2
percentile2=V3

detach(data)
data=read.table(file3)
attach(data)
waitTime3 = V2
percentile3=V3

plot(lambda, waitTime1, xlab="Lambda", ylab= "E[A]", col="red",type="l",ylim=c(0.0,16));
lines(lambda, waitTime2, col="blue");
lines(lambda, waitTime3, col="green");
plot(lambda, percentile1, xlab="Lambda", ylab= "90 percentile", col="red",type="l", ylim=c(0.0,37));
lines(lambda, percentile2, col="blue");
lines(lambda, percentile3, col="green");