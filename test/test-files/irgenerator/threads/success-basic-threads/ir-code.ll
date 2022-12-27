; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [25 x i8] c"Starting one thread ...\0A\00", align 1
@1 = private unnamed_addr constant [25 x i8] c"Hello from the thread 1\0A\00", align 1
@2 = private unnamed_addr constant [25 x i8] c"Hello from the thread 2\0A\00", align 1
@3 = private unnamed_addr constant [21 x i8] c"Hello from original\0A\00", align 1

declare i32 @usleep(i32)

define i32 @main() {
entry.l3:
  %result = alloca i32, align 4
  %0 = alloca {}, align 8
  %1 = alloca {}, align 8
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %6 = call i32 (ptr, ...) @printf(ptr @0)
  %7 = alloca ptr, align 8
  %8 = call i32 @pthread_create(ptr %7, ptr null, ptr @_thread0, ptr %0)
  %9 = alloca ptr, align 8
  %10 = call i32 @pthread_create(ptr %9, ptr null, ptr @_thread1, ptr %1)
  store i32 1000, ptr %2, align 4
  store i32 1000, ptr %3, align 4
  store i32 1000000, ptr %4, align 4
  %11 = load i32, ptr %4, align 4
  %12 = call i32 @usleep(i32 %11)
  store i32 %12, ptr %5, align 4
  %13 = call i32 (ptr, ...) @printf(ptr @3)
  %14 = load i32, ptr %result, align 4
  ret i32 %14
}

declare i32 @printf(ptr, ...)

define internal ptr @_thread0(ptr %0) {
thread.entry.l5:
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 500, ptr %1, align 4
  store i32 1000, ptr %2, align 4
  store i32 500000, ptr %3, align 4
  %5 = load i32, ptr %3, align 4
  %6 = call i32 @usleep(i32 %5)
  store i32 %6, ptr %4, align 4
  %7 = call i32 (ptr, ...) @printf(ptr @1)
  ret ptr null
}

declare i32 @pthread_create(ptr, ptr, ptr, ptr)

define internal ptr @_thread1(ptr %0) {
thread.entry.l9:
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 200, ptr %1, align 4
  store i32 1000, ptr %2, align 4
  store i32 200000, ptr %3, align 4
  %5 = load i32, ptr %3, align 4
  %6 = call i32 @usleep(i32 %5)
  store i32 %6, ptr %4, align 4
  %7 = call i32 (ptr, ...) @printf(ptr @2)
  ret ptr null
}