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
  %i = alloca i32, align 4
  %0 = alloca i1, align 1
  %1 = alloca { i32*, i32* }, align 8
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @0, i32 0, i32 0))
  store volatile i32 0, i32* %capturedVariable, align 4
  store i32 1, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.post, %entry
  %6 = load i32, i32* %i, align 4
  %7 = icmp sle i32 %6, 8
  store i1 %7, i1* %0, align 1
  %8 = load i1, i1* %0, align 1
  br i1 %8, label %for, label %for.end

for:                                              ; preds = %for.cond
  %9 = load i32, i32* %i, align 4
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @1, i32 0, i32 0), i32 %9)
  %11 = getelementptr inbounds { i32*, i32* }, { i32*, i32* }* %1, i32 0, i32 0
  store i32* %capturedVariable, i32** %11, align 8
  %12 = getelementptr inbounds { i32*, i32* }, { i32*, i32* }* %1, i32 0, i32 1
  store i32* %i, i32** %12, align 8
  %13 = alloca i8*, align 8
  %14 = bitcast { i32*, i32* }* %1 to i8*
  %15 = call i32 @pthread_create(i8** %13, i8* null, i8* (i8*)* @_thread0, i8* %14)
  br label %for.post

for.post:                                         ; preds = %for
  %16 = load i32, i32* %i, align 4
  %17 = add i32 %16, 1
  store i32 %17, i32* %i, align 4
  store i32 %16, i32* %2, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  store i32 1000000, i32* %3, align 4
  %18 = load i32, i32* %3, align 4
  %19 = call i32 @usleep(i32 %18)
  store i32 %19, i32* %4, align 4
  %20 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @3, i32 0, i32 0))
  %21 = load i32, i32* %result, align 4
  ret i32 %21
}

declare i32 @printf(i8*, ...)

define internal i8* @_thread0(i8* %0) {
entry:
  %1 = bitcast i8* %0 to { i32*, i32* }*
  %2 = getelementptr inbounds { i32*, i32* }, { i32*, i32* }* %1, i32 0, i32 0
  %3 = load i32*, i32** %2, align 8
  %4 = getelementptr inbounds { i32*, i32* }, { i32*, i32* }* %1, i32 0, i32 1
  %5 = load i32*, i32** %4, align 8
  %6 = load i32, i32* %5, align 4
  %7 = mul i32 100, %6
  %8 = mul i32 %7, 1000
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  store i32 %8, i32* %9, align 4
  %11 = load i32, i32* %9, align 4
  %12 = call i32 @usleep(i32 %11)
  store i32 %12, i32* %10, align 4
  %13 = load i32, i32* %3, align 4
  %14 = mul i32 %13, 2
  store volatile i32 %14, i32* %3, align 4
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @2, i32 0, i32 0))
  ret i8* null
}

declare i32 @pthread_create(i8**, i8*, i8* (i8*)*, i8*)
