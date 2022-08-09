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
  store i32 0, ptr %result, align 4
  %5 = call i32 (ptr, ...) @printf(ptr @0)
  store i32 0, ptr %capturedVariable, align 4
  store i32 1, ptr %i, align 4
  br label %for.cond.l6

for.l6:                                           ; preds = %for.cond.l6
  %6 = load i32, ptr %i, align 4
  %7 = call i32 (ptr, ...) @printf(ptr @1, i32 %6)
  %8 = getelementptr inbounds { ptr, ptr }, ptr %0, i32 0, i32 0
  store ptr %capturedVariable, ptr %8, align 8
  %9 = getelementptr inbounds { ptr, ptr }, ptr %0, i32 0, i32 1
  store ptr %i, ptr %9, align 8
  %10 = alloca ptr, align 8
  %11 = call i32 @pthread_create(ptr %10, ptr null, ptr @_thread0, ptr %0)
  br label %for.inc.l6

for.inc.l6:                                       ; preds = %for.l6
  %12 = load i32, ptr %i, align 4
  %13 = add i32 %12, 1
  store i32 %13, ptr %i, align 4
  store i32 %12, ptr %1, align 4
  br label %for.cond.l6

for.cond.l6:                                      ; preds = %for.inc.l6, %entry.l3
  %14 = load i32, ptr %i, align 4
  %15 = icmp sle i32 %14, 8
  store i1 %15, ptr %2, align 1
  %16 = load i1, ptr %2, align 1
  br i1 %16, label %for.l6, label %for.end.l6

for.end.l6:                                       ; preds = %for.cond.l6
  store i32 1000000, ptr %3, align 4
  %17 = load i32, ptr %3, align 4
  %18 = call i32 @usleep(i32 %17)
  store i32 %18, ptr %4, align 4
  %19 = call i32 (ptr, ...) @printf(ptr @3)
  %20 = load i32, ptr %result, align 4
  ret i32 %20
}

declare i32 @printf(ptr, ...)

define internal ptr @_thread0(ptr %0) {
thread.entry.l8:
  %1 = getelementptr inbounds { ptr, ptr }, ptr %0, i32 0, i32 0
  %2 = load ptr, ptr %1, align 8
  %3 = getelementptr inbounds { ptr, ptr }, ptr %0, i32 0, i32 1
  %4 = load ptr, ptr %3, align 8
  %5 = load i32, ptr %4, align 4
  %6 = mul i32 100, %5
  %7 = mul i32 %6, 1000
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  store i32 %7, ptr %8, align 4
  %10 = load i32, ptr %8, align 4
  %11 = call i32 @usleep(i32 %10)
  store i32 %11, ptr %9, align 4
  %12 = load i32, ptr %2, align 4
  %13 = mul i32 %12, 2
  store volatile i32 %13, ptr %2, align 4
  %14 = call i32 (ptr, ...) @printf(ptr @2)
  ret ptr null
}

declare i32 @pthread_create(ptr, ptr, ptr, ptr)
