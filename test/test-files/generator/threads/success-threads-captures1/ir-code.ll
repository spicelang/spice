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
  store i32 0, ptr %result, align 4
  %0 = call i32 (ptr, ...) @printf(ptr @0)
  %i = alloca i32, align 4
  store i32 1, ptr %i, align 4
  br label %for.cond.l5

for.l5:                                           ; preds = %for.cond.l5
  %1 = load i32, ptr %i, align 4
  %2 = call i32 (ptr, ...) @printf(ptr @1, i32 %1)
  %3 = alloca { ptr }, align 8
  %4 = getelementptr inbounds { ptr }, ptr %3, i32 0, i32 0
  store ptr %i, ptr %4, align 8
  %5 = alloca ptr, align 8
  %6 = call i32 @pthread_create(ptr %5, ptr null, ptr @_thread0, ptr %3)
  br label %for.inc.l5

for.inc.l5:                                       ; preds = %for.l5
  %7 = load i32, ptr %i, align 4
  %8 = add i32 %7, 1
  store i32 %8, ptr %i, align 4
  %9 = alloca i32, align 4
  store i32 %7, ptr %9, align 4
  br label %for.cond.l5

for.cond.l5:                                      ; preds = %for.inc.l5, %entry.l3
  %10 = load i32, ptr %i, align 4
  %11 = icmp sle i32 %10, 8
  %12 = alloca i1, align 1
  store i1 %11, ptr %12, align 1
  %13 = load i1, ptr %12, align 1
  br i1 %13, label %for.l5, label %for.end.l5

for.end.l5:                                       ; preds = %for.cond.l5
  %14 = alloca i32, align 4
  store i32 1000000, ptr %14, align 4
  %15 = load i32, ptr %14, align 4
  %16 = call i32 @usleep(i32 %15)
  %17 = alloca i32, align 4
  store i32 %16, ptr %17, align 4
  %18 = call i32 (ptr, ...) @printf(ptr @3)
  %19 = load i32, ptr %result, align 4
  ret i32 %19
}

declare i32 @printf(ptr, ...)

define internal ptr @_thread0(ptr %0) {
thread.entry.l7:
  %1 = getelementptr inbounds { ptr }, ptr %0, i32 0, i32 0
  %2 = load ptr, ptr %1, align 8
  %3 = load i32, ptr %2, align 4
  %4 = mul i32 100, %3
  %5 = mul i32 %4, 1000
  %6 = alloca i32, align 4
  store i32 %5, ptr %6, align 4
  %7 = load i32, ptr %6, align 4
  %8 = call i32 @usleep(i32 %7)
  %9 = alloca i32, align 4
  store i32 %8, ptr %9, align 4
  %10 = call i32 (ptr, ...) @printf(ptr @2)
  ret ptr null
}

declare i32 @pthread_create(ptr, ptr, ptr, ptr)
