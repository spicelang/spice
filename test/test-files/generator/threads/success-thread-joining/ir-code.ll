; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@1 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@2 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@3 = private unnamed_addr constant [19 x i8] c"Thread 1 finished\0A\00", align 1
@4 = private unnamed_addr constant [19 x i8] c"Thread 2 finished\0A\00", align 1
@5 = private unnamed_addr constant [19 x i8] c"Thread 3 finished\0A\00", align 1
@6 = private unnamed_addr constant [18 x i8] c"Program finished\0A\00", align 1

declare i32 @usleep(i32)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %t1 = alloca i8*, align 8
  %t2 = alloca i8*, align 8
  %t3 = alloca i8*, align 8
  %0 = alloca {}, align 8
  %1 = alloca { i8**, i8** }, align 8
  %2 = alloca {}, align 8
  %3 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i8* getelementptr inbounds ([1 x i8], [1 x i8]* @0, i32 0, i32 0), i8** %t1, align 8
  store i8* getelementptr inbounds ([1 x i8], [1 x i8]* @1, i32 0, i32 0), i8** %t2, align 8
  store i8* getelementptr inbounds ([1 x i8], [1 x i8]* @2, i32 0, i32 0), i8** %t3, align 8
  %4 = alloca i8*, align 8
  %5 = bitcast {}* %0 to i8*
  %6 = call i32 @pthread_create(i8** %4, i8* null, i8* (i8*)* @_thread0, i8* %5)
  %7 = load i8*, i8** %4, align 8
  store i8* %7, i8** %t1, align 8
  %8 = getelementptr inbounds { i8**, i8** }, { i8**, i8** }* %1, i32 0, i32 0
  store i8** %t1, i8*** %8, align 8
  %9 = getelementptr inbounds { i8**, i8** }, { i8**, i8** }* %1, i32 0, i32 1
  store i8** %t3, i8*** %9, align 8
  %10 = alloca i8*, align 8
  %11 = bitcast { i8**, i8** }* %1 to i8*
  %12 = call i32 @pthread_create(i8** %10, i8* null, i8* (i8*)* @_thread1, i8* %11)
  %13 = load i8*, i8** %10, align 8
  store i8* %13, i8** %t2, align 8
  %14 = alloca i8*, align 8
  %15 = bitcast {}* %2 to i8*
  %16 = call i32 @pthread_create(i8** %14, i8* null, i8* (i8*)* @_thread2, i8* %15)
  %17 = load i8*, i8** %14, align 8
  store i8* %17, i8** %t3, align 8
  %18 = load i8*, i8** %t1, align 8
  %19 = call i32 @pthread_join(i8* %18, i8** null)
  %20 = load i8*, i8** %t2, align 8
  %21 = call i32 @pthread_join(i8* %20, i8** null)
  %22 = load i8*, i8** %t3, align 8
  %23 = call i32 @pthread_join(i8* %22, i8** null)
  store i32 3, i32* %3, align 4
  %24 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @6, i32 0, i32 0))
  %25 = load i32, i32* %result, align 4
  ret i32 %25
}

define internal i8* @_thread0(i8* %0) {
entry:
  %1 = bitcast i8* %0 to {}*
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 300000, i32* %2, align 4
  %4 = load i32, i32* %2, align 4
  %5 = call i32 @usleep(i32 %4)
  store i32 %5, i32* %3, align 4
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @3, i32 0, i32 0))
  ret i8* null
}

declare i32 @printf(i8*, ...)

declare i32 @pthread_create(i8**, i8*, i8* (i8*)*, i8*)

define internal i8* @_thread1(i8* %0) {
entry:
  %1 = bitcast i8* %0 to { i8**, i8** }*
  %2 = getelementptr inbounds { i8**, i8** }, { i8**, i8** }* %1, i32 0, i32 0
  %3 = load i8**, i8*** %2, align 8
  %4 = getelementptr inbounds { i8**, i8** }, { i8**, i8** }* %1, i32 0, i32 1
  %5 = load i8**, i8*** %4, align 8
  %6 = load i8*, i8** %3, align 8
  %7 = call i32 @pthread_join(i8* %6, i8** null)
  %8 = load i8*, i8** %5, align 8
  %9 = call i32 @pthread_join(i8* %8, i8** null)
  %10 = alloca i32, align 4
  store i32 2, i32* %10, align 4
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @4, i32 0, i32 0))
  ret i8* null
}

declare i32 @pthread_join(i8*, i8**)

define internal i8* @_thread2(i8* %0) {
entry:
  %1 = bitcast i8* %0 to {}*
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 200000, i32* %2, align 4
  %4 = load i32, i32* %2, align 4
  %5 = call i32 @usleep(i32 %4)
  store i32 %5, i32* %3, align 4
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @5, i32 0, i32 0))
  ret i8* null
}
