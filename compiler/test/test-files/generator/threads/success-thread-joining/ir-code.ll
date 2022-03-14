; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [10 x i8] c"Thread 1\0A\00", align 1
@1 = private unnamed_addr constant [19 x i8] c"Thread 1 finished\0A\00", align 1
@2 = private unnamed_addr constant [10 x i8] c"Thread 2\0A\00", align 1
@3 = private unnamed_addr constant [19 x i8] c"Thread 2 finished\0A\00", align 1
@4 = private unnamed_addr constant [10 x i8] c"Thread 3\0A\00", align 1
@5 = private unnamed_addr constant [19 x i8] c"Thread 3 finished\0A\00", align 1

declare i32 @usleep(i32)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %t1 = alloca i32, align 4
  %t2 = alloca i32, align 4
  %t3 = alloca i32, align 4
  %0 = alloca {}, align 8
  %1 = alloca { i32*, i32* }, align 8
  %2 = alloca {}, align 8
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %7 = alloca i32, align 4
  %8 = bitcast {}* %0 to i8*
  %9 = call i32 @pthread_create(i32* %7, i8* null, i8* (i8*)* @_thread0, i8* %8)
  %10 = load i32, i32* %7, align 4
  store i32 %10, i32* %t1, align 4
  %11 = getelementptr inbounds { i32*, i32* }, { i32*, i32* }* %1, i32 0, i32 0
  store i32* %t1, i32** %11, align 8
  %12 = getelementptr inbounds { i32*, i32* }, { i32*, i32* }* %1, i32 0, i32 1
  store i32* %t3, i32** %12, align 8
  %13 = alloca i32, align 4
  %14 = bitcast { i32*, i32* }* %1 to i8*
  %15 = call i32 @pthread_create(i32* %13, i8* null, i8* (i8*)* @_thread1, i8* %14)
  %16 = load i32, i32* %13, align 4
  store i32 %16, i32* %t2, align 4
  %17 = alloca i32, align 4
  %18 = bitcast {}* %2 to i8*
  %19 = call i32 @pthread_create(i32* %17, i8* null, i8* (i8*)* @_thread2, i8* %18)
  %20 = load i32, i32* %17, align 4
  store i32 %20, i32* %t3, align 4
  store i32 1000, i32* %3, align 4
  %21 = load i32, i32* %3, align 4
  store i32 1000, i32* %4, align 4
  %22 = load i32, i32* %4, align 4
  %23 = mul i32 %21, %22
  store i32 %23, i32* %5, align 4
  %24 = load i32, i32* %5, align 4
  %25 = call i32 @usleep(i32 %24)
  store i32 %25, i32* %6, align 4
  %26 = load i32, i32* %result, align 4
  ret i32 %26
}

define internal i8* @_thread0(i8* %0) {
entry:
  %1 = bitcast i8* %0 to {}*
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0))
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store i32 300, i32* %3, align 4
  %7 = load i32, i32* %3, align 4
  store i32 1000, i32* %4, align 4
  %8 = load i32, i32* %4, align 4
  %9 = mul i32 %7, %8
  store i32 %9, i32* %5, align 4
  %10 = load i32, i32* %5, align 4
  %11 = call i32 @usleep(i32 %10)
  store i32 %11, i32* %6, align 4
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @1, i32 0, i32 0))
  ret i8* null
}

declare i32 @printf(i8*, ...)

declare i32 @pthread_create(i32*, i8*, i8* (i8*)*, i8*)

define internal i8* @_thread1(i8* %0) {
entry:
  %1 = bitcast i8* %0 to { i32*, i32* }*
  %2 = getelementptr inbounds { i32*, i32* }, { i32*, i32* }* %1, i32 0, i32 0
  %3 = load i32*, i32** %2, align 8
  %4 = getelementptr inbounds { i32*, i32* }, { i32*, i32* }* %1, i32 0, i32 1
  %5 = load i32*, i32** %4, align 8
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @2, i32 0, i32 0))
  %7 = load i32, i32* %3, align 4
  %8 = inttoptr i32 %7 to i8*
  %9 = call i32 @pthread_join(i8* %8, i8** null)
  %10 = load i32, i32* %5, align 4
  %11 = inttoptr i32 %10 to i8*
  %12 = call i32 @pthread_join(i8* %11, i8** null)
  %13 = alloca i32, align 4
  store i32 2, i32* %13, align 4
  %14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @3, i32 0, i32 0))
  ret i8* null
}

declare i32 @pthread_join(i8*, i8**)

define internal i8* @_thread2(i8* %0) {
entry:
  %1 = bitcast i8* %0 to {}*
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @4, i32 0, i32 0))
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store i32 200, i32* %3, align 4
  %7 = load i32, i32* %3, align 4
  store i32 1000, i32* %4, align 4
  %8 = load i32, i32* %4, align 4
  %9 = mul i32 %7, %8
  store i32 %9, i32* %5, align 4
  %10 = load i32, i32* %5, align 4
  %11 = call i32 @usleep(i32 %10)
  store i32 %11, i32* %6, align 4
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @5, i32 0, i32 0))
  ret i8* null
}
