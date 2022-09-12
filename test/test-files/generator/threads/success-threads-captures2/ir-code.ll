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
  %capturedVariable = alloca i32, align 4
  %i = alloca i32, align 4
  %0 = alloca { ptr, ptr }, align 8
  %1 = alloca i32, align 4
  %2 = alloca i1, align 1
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %7 = call i32 (ptr, ...) @printf(ptr @0)
  store i32 0, ptr %capturedVariable, align 4
  store i32 1, ptr %i, align 4
  br label %for.cond.l6

for.l6:                                           ; preds = %for.cond.l6
  %8 = load i32, ptr %i, align 4
  %9 = call i32 (ptr, ...) @printf(ptr @1, i32 %8)
  %10 = getelementptr inbounds { ptr, ptr }, ptr %0, i32 0, i32 0
  store ptr %capturedVariable, ptr %10, align 8
  %11 = getelementptr inbounds { ptr, ptr }, ptr %0, i32 0, i32 1
  store ptr %i, ptr %11, align 8
  %12 = alloca ptr, align 8
  %13 = call i32 @pthread_create(ptr %12, ptr null, ptr @_thread0, ptr %0)
  br label %for.inc.l6

for.inc.l6:                                       ; preds = %for.l6
  %14 = load i32, ptr %i, align 4
  %15 = add i32 %14, 1
  store i32 %15, ptr %i, align 4
  store i32 %14, ptr %1, align 4
  br label %for.cond.l6

for.cond.l6:                                      ; preds = %for.inc.l6, %entry.l3
  %16 = load i32, ptr %i, align 4
  %17 = icmp sle i32 %16, 8
  store i1 %17, ptr %2, align 1
  %18 = load i1, ptr %2, align 1
  br i1 %18, label %for.l6, label %for.end.l6

for.end.l6:                                       ; preds = %for.cond.l6
  store i32 1000, ptr %3, align 4
  store i32 1000, ptr %4, align 4
  store i32 1000000, ptr %5, align 4
  %19 = load i32, ptr %5, align 4
  %20 = call i32 @usleep(i32 %19)
  store i32 %20, ptr %6, align 4
  %21 = call i32 (ptr, ...) @printf(ptr @3)
  %22 = load i32, ptr %result, align 4
  ret i32 %22
}

declare i32 @printf(ptr, ...)

define internal ptr @_thread0(ptr %0) {
thread.entry.l8:
  %1 = getelementptr inbounds { ptr, ptr }, ptr %0, i32 0, i32 0
  %2 = load ptr, ptr %1, align 8
  %3 = getelementptr inbounds { ptr, ptr }, ptr %0, i32 0, i32 1
  %4 = load ptr, ptr %3, align 8
  %5 = load i32, ptr %4, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  store i32 100, ptr %6, align 4
  %11 = mul i32 100, %5
  store i32 100, ptr %7, align 4
  store i32 1000, ptr %8, align 4
  %12 = mul i32 %11, 1000
  store i32 %12, ptr %9, align 4
  %13 = load i32, ptr %9, align 4
  %14 = call i32 @usleep(i32 %13)
  store i32 %14, ptr %10, align 4
  %15 = load i32, ptr %2, align 4
  %16 = mul i32 %15, 2
  store volatile i32 %16, ptr %2, align 4
  %17 = call i32 (ptr, ...) @printf(ptr @2)
  ret ptr null
}

declare i32 @pthread_create(ptr, ptr, ptr, ptr)
