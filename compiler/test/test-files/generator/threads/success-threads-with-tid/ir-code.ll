; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [22 x i8] c"Starting threads ...\0A\00", align 1
@1 = private unnamed_addr constant [26 x i8] c"Hello from the thread %d\0A\00", align 1
@2 = private unnamed_addr constant [21 x i8] c"Hello from original\0A\00", align 1

declare i32 @printf(i8*, ...)

declare i32 @usleep(i32)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i1, align 1
  %3 = alloca { i32 }, align 8
  %4 = alloca i8, align 1
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @0, i32 0, i32 0))
  store i32 0, i32* %0, align 4
  %11 = load i32, i32* %0, align 4
  store i32 %11, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.post, %entry
  store i32 10, i32* %1, align 4
  %12 = load i32, i32* %i, align 4
  %13 = load i32, i32* %1, align 4
  %14 = icmp slt i32 %12, %13
  store i1 %14, i1* %2, align 1
  %15 = load i1, i1* %2, align 1
  br i1 %15, label %for, label %for.end

for:                                              ; preds = %for.cond
  %16 = load i32, i32* %i, align 4
  %17 = icmp eq i32 %16, -1
  br i1 %17, label %thread.if.then, label %thread.if.end

thread.if.then:                                   ; preds = %for
  store i32 -3, i32* %i, align 4
  br label %thread.if.end

thread.if.end:                                    ; preds = %thread.if.then, %for
  %18 = load i32, i32* %i, align 4
  %19 = getelementptr inbounds { i32 }, { i32 }* %3, i32 0, i32 0
  store i32 %18, i32* %19, align 4
  %20 = bitcast { i32 }* %3 to i8*
  %21 = call i32 @pthread_create(i8* %4, i8* null, i8* (i8*)* @3, i8* %20)
  br label %for.post

for.post:                                         ; preds = %thread.if.end
  %22 = load i32, i32* %i, align 4
  %23 = add i32 %22, 1
  store i32 %23, i32* %i, align 4
  store i32 %22, i32* %5, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  store i32 1000, i32* %6, align 4
  %24 = load i32, i32* %6, align 4
  store i32 1000, i32* %7, align 4
  %25 = load i32, i32* %7, align 4
  %26 = mul i32 %24, %25
  store i32 %26, i32* %8, align 4
  %27 = load i32, i32* %8, align 4
  %28 = call i32 @usleep(i32 %27)
  store i32 %28, i32* %9, align 4
  %29 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @2, i32 0, i32 0))
  %30 = load i32, i32* %result, align 4
  ret i32 %30
}

define internal i8* @3(i8* %0) {
entry:
  %tid = alloca i8*, align 8
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i8* %0, i8** %tid, align 8
  store i32 200, i32* %1, align 4
  %5 = load i32, i32* %1, align 4
  store i32 1000, i32* %2, align 4
  %6 = load i32, i32* %2, align 4
  %7 = mul i32 %5, %6
  store i32 %7, i32* %3, align 4
  %8 = load i32, i32* %3, align 4
  %9 = call i32 @usleep(i32 %8)
  store i32 %9, i32* %4, align 4
  %10 = load i8*, i8** %tid, align 8
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([26 x i8], [26 x i8]* @1, i32 0, i32 0), i8* %10)
  ret i8* null
}

declare i32 @pthread_create(i8*, i8*, i8* (i8*)*, i8*)
