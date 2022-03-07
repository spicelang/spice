; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [25 x i8] c"Starting one thread ...\0A\00", align 1
@1 = private unnamed_addr constant [25 x i8] c"Hello from the thread 1\0A\00", align 1
@2 = private unnamed_addr constant [25 x i8] c"Hello from the thread 2\0A\00", align 1
@3 = private unnamed_addr constant [21 x i8] c"Hello from original\0A\00", align 1

declare i32 @printf(i8*, ...)

declare i32 @usleep(i32)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %0 = alloca i32, align 4
  %1 = alloca i8, align 1
  %2 = alloca {}, align 8
  %3 = alloca i32, align 4
  %4 = alloca i8, align 1
  %5 = alloca {}, align 8
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @0, i32 0, i32 0))
  store i32 1, i32* %0, align 4
  %11 = load i32, i32* %0, align 4
  %12 = mul i32 %11, -1
  store i32 %12, i32* %0, align 4
  %13 = load i32, i32* %0, align 4
  %14 = icmp eq i32 %13, -1
  br i1 %14, label %thread.if.then, label %thread.if.end

thread.if.then:                                   ; preds = %entry
  store i32 -1, i32* %0, align 4
  br label %thread.if.end

thread.if.end:                                    ; preds = %thread.if.then, %entry
  %15 = bitcast {}* %2 to i8*
  %16 = call i32 @pthread_create(i8* %1, i8* null, i8* (i8*)* @4, i8* %15)
  store i32 1, i32* %3, align 4
  %17 = load i32, i32* %3, align 4
  %18 = mul i32 %17, -1
  store i32 %18, i32* %3, align 4
  %19 = load i32, i32* %3, align 4
  %20 = icmp eq i32 %19, -1
  br i1 %20, label %thread.if.then1, label %thread.if.end2

thread.if.then1:                                  ; preds = %thread.if.end
  store i32 -2, i32* %3, align 4
  br label %thread.if.end2

thread.if.end2:                                   ; preds = %thread.if.then1, %thread.if.end
  %21 = bitcast {}* %5 to i8*
  %22 = call i32 @pthread_create(i8* %4, i8* null, i8* (i8*)* @5, i8* %21)
  store i32 1000, i32* %6, align 4
  %23 = load i32, i32* %6, align 4
  store i32 1000, i32* %7, align 4
  %24 = load i32, i32* %7, align 4
  %25 = mul i32 %23, %24
  store i32 %25, i32* %8, align 4
  %26 = load i32, i32* %8, align 4
  %27 = call i32 @usleep(i32 %26)
  store i32 %27, i32* %9, align 4
  %28 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @3, i32 0, i32 0))
  %29 = load i32, i32* %result, align 4
  ret i32 %29
}

define internal i8* @4(i8* %0) {
entry:
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 500, i32* %1, align 4
  %5 = load i32, i32* %1, align 4
  store i32 1000, i32* %2, align 4
  %6 = load i32, i32* %2, align 4
  %7 = mul i32 %5, %6
  store i32 %7, i32* %3, align 4
  %8 = load i32, i32* %3, align 4
  %9 = call i32 @usleep(i32 %8)
  store i32 %9, i32* %4, align 4
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @1, i32 0, i32 0))
  ret i8* null
}

declare i32 @pthread_create(i8*, i8*, i8* (i8*)*, i8*)

define internal i8* @5(i8* %0) {
entry:
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 200, i32* %1, align 4
  %5 = load i32, i32* %1, align 4
  store i32 1000, i32* %2, align 4
  %6 = load i32, i32* %2, align 4
  %7 = mul i32 %5, %6
  store i32 %7, i32* %3, align 4
  %8 = load i32, i32* %3, align 4
  %9 = call i32 @usleep(i32 %8)
  store i32 %9, i32* %4, align 4
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @2, i32 0, i32 0))
  ret i8* null
}
