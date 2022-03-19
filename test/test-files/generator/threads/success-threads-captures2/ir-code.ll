; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [22 x i8] c"Starting threads ...\0A\00", align 1
@1 = private unnamed_addr constant [24 x i8] c"Starting thread %d ...\0A\00", align 1
@2 = private unnamed_addr constant [23 x i8] c"Hello from the thread\0A\00", align 1
@3 = private unnamed_addr constant [21 x i8] c"Hello from original\0A\00", align 1

declare i32 @usleep(i32)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %capturedVariable = alloca i32, align 4
  %0 = alloca i32, align 4
  %i = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i1, align 1
  %4 = alloca { i32*, i32* }, align 8
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @0, i32 0, i32 0))
  store i32 0, i32* %0, align 4
  %11 = load i32, i32* %0, align 4
  store volatile i32 %11, i32* %capturedVariable, align 4
  store i32 1, i32* %1, align 4
  %12 = load i32, i32* %1, align 4
  store i32 %12, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.post, %entry
  store i32 8, i32* %2, align 4
  %13 = load i32, i32* %i, align 4
  %14 = load i32, i32* %2, align 4
  %15 = icmp sle i32 %13, %14
  store i1 %15, i1* %3, align 1
  %16 = load i1, i1* %3, align 1
  br i1 %16, label %for, label %for.end

for:                                              ; preds = %for.cond
  %17 = load i32, i32* %i, align 4
  %18 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @1, i32 0, i32 0), i32 %17)
  %19 = getelementptr inbounds { i32*, i32* }, { i32*, i32* }* %4, i32 0, i32 0
  store i32* %capturedVariable, i32** %19, align 8
  %20 = getelementptr inbounds { i32*, i32* }, { i32*, i32* }* %4, i32 0, i32 1
  store i32* %i, i32** %20, align 8
  %21 = alloca i8*, align 8
  %22 = bitcast { i32*, i32* }* %4 to i8*
  %23 = call i32 @pthread_create(i8** %21, i8* null, i8* (i8*)* @_thread0, i8* %22)
  br label %for.post

for.post:                                         ; preds = %for
  %24 = load i32, i32* %i, align 4
  %25 = add i32 %24, 1
  store i32 %25, i32* %i, align 4
  store i32 %24, i32* %5, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  store i32 1000, i32* %6, align 4
  %26 = load i32, i32* %6, align 4
  store i32 1000, i32* %7, align 4
  %27 = load i32, i32* %7, align 4
  %28 = mul i32 %26, %27
  store i32 %28, i32* %8, align 4
  %29 = load i32, i32* %8, align 4
  %30 = call i32 @usleep(i32 %29)
  store i32 %30, i32* %9, align 4
  %31 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @3, i32 0, i32 0))
  %32 = load i32, i32* %result, align 4
  ret i32 %32
}

declare i32 @printf(i8*, ...)

define internal i8* @_thread0(i8* %0) {
entry:
  %1 = bitcast i8* %0 to { i32*, i32* }*
  %2 = getelementptr inbounds { i32*, i32* }, { i32*, i32* }* %1, i32 0, i32 0
  %3 = load i32*, i32** %2, align 8
  %4 = getelementptr inbounds { i32*, i32* }, { i32*, i32* }* %1, i32 0, i32 1
  %5 = load i32*, i32** %4, align 8
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  store i32 100, i32* %6, align 4
  %11 = load i32, i32* %6, align 4
  %12 = load i32, i32* %5, align 4
  %13 = mul i32 %11, %12
  store i32 1000, i32* %7, align 4
  %14 = load i32, i32* %7, align 4
  %15 = mul i32 %13, %14
  store i32 %15, i32* %8, align 4
  %16 = load i32, i32* %8, align 4
  %17 = call i32 @usleep(i32 %16)
  store i32 %17, i32* %9, align 4
  store i32 2, i32* %10, align 4
  %18 = load i32, i32* %10, align 4
  %19 = load i32, i32* %3, align 4
  %20 = mul i32 %19, %18
  store volatile i32 %20, i32* %3, align 4
  %21 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @2, i32 0, i32 0))
  ret i8* null
}

declare i32 @pthread_create(i8**, i8*, i8* (i8*)*, i8*)
