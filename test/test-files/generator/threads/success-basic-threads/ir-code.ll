; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [25 x i8] c"Starting one thread ...\0A\00", align 1
@1 = private unnamed_addr constant [25 x i8] c"Hello from the thread 1\0A\00", align 1
@2 = private unnamed_addr constant [25 x i8] c"Hello from the thread 2\0A\00", align 1
@3 = private unnamed_addr constant [21 x i8] c"Hello from original\0A\00", align 1

declare i32 @usleep(i32)

define i32 @main() {
entry.l3:
  %result = alloca i32, align 4
  %0 = alloca {}, align 8
  %1 = alloca {}, align 8
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @0, i32 0, i32 0))
  %5 = alloca i8*, align 8
  %6 = bitcast {}* %0 to i8*
  %7 = call i32 @pthread_create(i8** %5, i8* null, i8* (i8*)* @_thread0, i8* %6)
  %8 = alloca i8*, align 8
  %9 = bitcast {}* %1 to i8*
  %10 = call i32 @pthread_create(i8** %8, i8* null, i8* (i8*)* @_thread1, i8* %9)
  store i32 1000000, i32* %2, align 4
  %11 = load i32, i32* %2, align 4
  %12 = call i32 @usleep(i32 %11)
  store i32 %12, i32* %3, align 4
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @3, i32 0, i32 0))
  %14 = load i32, i32* %result, align 4
  ret i32 %14
}

declare i32 @printf(i8*, ...)

define internal i8* @_thread0(i8* %0) {
thread.entry.l5:
  %1 = bitcast i8* %0 to {}*
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 500000, i32* %2, align 4
  %4 = load i32, i32* %2, align 4
  %5 = call i32 @usleep(i32 %4)
  store i32 %5, i32* %3, align 4
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @1, i32 0, i32 0))
  ret i8* null
}

declare i32 @pthread_create(i8**, i8*, i8* (i8*)*, i8*)

define internal i8* @_thread1(i8* %0) {
thread.entry.l9:
  %1 = bitcast i8* %0 to {}*
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 200000, i32* %2, align 4
  %4 = load i32, i32* %2, align 4
  %5 = call i32 @usleep(i32 %4)
  store i32 %5, i32* %3, align 4
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @2, i32 0, i32 0))
  ret i8* null
}
