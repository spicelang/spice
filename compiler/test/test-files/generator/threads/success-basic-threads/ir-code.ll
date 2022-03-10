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
entry:
  %result = alloca i32, align 4
  %0 = alloca {}, align 8
  %1 = alloca {}, align 8
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @0, i32 0, i32 0))
  %7 = alloca i16, align 2
  %8 = bitcast {}* %0 to i8*
  %9 = call i32 @pthread_create(i16* %7, i8* null, i8* (i8*)* @_thread0, i8* %8)
  %10 = alloca i16, align 2
  %11 = bitcast {}* %1 to i8*
  %12 = call i32 @pthread_create(i16* %10, i8* null, i8* (i8*)* @_thread1, i8* %11)
  store i32 1000, i32* %2, align 4
  %13 = load i32, i32* %2, align 4
  store i32 1000, i32* %3, align 4
  %14 = load i32, i32* %3, align 4
  %15 = mul i32 %13, %14
  store i32 %15, i32* %4, align 4
  %16 = load i32, i32* %4, align 4
  %17 = call i32 @usleep(i32 %16)
  store i32 %17, i32* %5, align 4
  %18 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @3, i32 0, i32 0))
  %19 = load i32, i32* %result, align 4
  ret i32 %19
}

declare i32 @printf(i8*, ...)

define internal i8* @_thread0(i8* %0) {
entry:
  %1 = bitcast i8* %0 to {}*
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 500, i32* %2, align 4
  %6 = load i32, i32* %2, align 4
  store i32 1000, i32* %3, align 4
  %7 = load i32, i32* %3, align 4
  %8 = mul i32 %6, %7
  store i32 %8, i32* %4, align 4
  %9 = load i32, i32* %4, align 4
  %10 = call i32 @usleep(i32 %9)
  store i32 %10, i32* %5, align 4
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @1, i32 0, i32 0))
  ret i8* null
}

declare i32 @pthread_create(i16*, i8*, i8* (i8*)*, i8*)

define internal i8* @_thread1(i8* %0) {
entry:
  %1 = bitcast i8* %0 to {}*
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 200, i32* %2, align 4
  %6 = load i32, i32* %2, align 4
  store i32 1000, i32* %3, align 4
  %7 = load i32, i32* %3, align 4
  %8 = mul i32 %6, %7
  store i32 %8, i32* %4, align 4
  %9 = load i32, i32* %4, align 4
  %10 = call i32 @usleep(i32 %9)
  store i32 %10, i32* %5, align 4
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @2, i32 0, i32 0))
  ret i8* null
}
