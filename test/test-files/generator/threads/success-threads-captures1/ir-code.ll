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
  %0 = alloca { ptr }, align 8
  %1 = alloca i32, align 4
  %2 = alloca i1, align 1
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %5 = call i32 (ptr, ...) @printf(ptr @0)
  store i32 1, ptr %i, align 4
  br label %for.cond.l5

for.l5:                                           ; preds = %for.cond.l5
  %6 = load i32, ptr %i, align 4
  %7 = call i32 (ptr, ...) @printf(ptr @1, i32 %6)
  %8 = getelementptr inbounds { ptr }, ptr %0, i32 0, i32 0
  store ptr %i, ptr %8, align 8
  %9 = alloca ptr, align 8
  %10 = call i32 @pthread_create(ptr %9, ptr null, ptr @_thread0, ptr %0)
  br label %for.inc.l5

for.inc.l5:                                       ; preds = %for.l5
  %11 = load i32, ptr %i, align 4
  %12 = add i32 %11, 1
  store i32 %12, ptr %i, align 4
  store i32 %11, ptr %1, align 4
  br label %for.cond.l5

for.cond.l5:                                      ; preds = %for.inc.l5, %entry.l3
  %13 = load i32, ptr %i, align 4
  %14 = icmp sle i32 %13, 8
  store i1 %14, ptr %2, align 1
  %15 = load i1, ptr %2, align 1
  br i1 %15, label %for.l5, label %for.end.l5

for.end.l5:                                       ; preds = %for.cond.l5
  store i32 1000000, ptr %3, align 4
  %16 = load i32, ptr %3, align 4
  %17 = call i32 @usleep(i32 %16)
  store i32 %17, ptr %4, align 4
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
  %7 = alloca i32, align 4
  store i32 %5, ptr %6, align 4
  %8 = load i32, ptr %6, align 4
  %9 = call i32 @usleep(i32 %8)
  store i32 %9, ptr %7, align 4
  %10 = call i32 (ptr, ...) @printf(ptr @2)
  ret ptr null
}

declare i32 @pthread_create(ptr, ptr, ptr, ptr)
