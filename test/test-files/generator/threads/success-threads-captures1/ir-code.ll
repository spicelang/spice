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
entry.l3:
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  %0 = alloca { i32* }, align 8
  %1 = alloca i32, align 4
  %2 = alloca i1, align 1
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @0, i32 0, i32 0))
  store i32 1, i32* %i, align 4
  br label %for.cond.l5

for.l5:                                           ; preds = %for.cond.l5
  %6 = load i32, i32* %i, align 4
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @1, i32 0, i32 0), i32 %6)
  %8 = getelementptr inbounds { i32* }, { i32* }* %0, i32 0, i32 0
  store i32* %i, i32** %8, align 8
  %9 = alloca i8*, align 8
  %10 = bitcast { i32* }* %0 to i8*
  %11 = call i32 @pthread_create(i8** %9, i8* null, i8* (i8*)* @_thread0, i8* %10)
  br label %for.inc.l5

for.inc.l5:                                       ; preds = %for.l5
  %12 = load i32, i32* %i, align 4
  %13 = add i32 %12, 1
  store i32 %13, i32* %i, align 4
  store i32 %12, i32* %1, align 4
  br label %for.cond.l5

for.cond.l5:                                      ; preds = %for.inc.l5, %entry.l3
  %14 = load i32, i32* %i, align 4
  %15 = icmp sle i32 %14, 8
  store i1 %15, i1* %2, align 1
  %16 = load i1, i1* %2, align 1
  br i1 %16, label %for.l5, label %for.end.l5

for.end.l5:                                       ; preds = %for.cond.l5
  store i32 1000000, i32* %3, align 4
  %17 = load i32, i32* %3, align 4
  %18 = call i32 @usleep(i32 %17)
  store i32 %18, i32* %4, align 4
  %19 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @3, i32 0, i32 0))
  %20 = load i32, i32* %result, align 4
  ret i32 %20
}

declare i32 @printf(i8*, ...)

define internal i8* @_thread0(i8* %0) {
thread.entry.l7:
  %1 = bitcast i8* %0 to { i32* }*
  %2 = getelementptr inbounds { i32* }, { i32* }* %1, i32 0, i32 0
  %3 = load i32*, i32** %2, align 8
  %4 = load i32, i32* %3, align 4
  %5 = mul i32 100, %4
  %6 = mul i32 %5, 1000
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  store i32 %6, i32* %7, align 4
  %9 = load i32, i32* %7, align 4
  %10 = call i32 @usleep(i32 %9)
  store i32 %10, i32* %8, align 4
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @2, i32 0, i32 0))
  ret i8* null
}

declare i32 @pthread_create(i8**, i8*, i8* (i8*)*, i8*)
