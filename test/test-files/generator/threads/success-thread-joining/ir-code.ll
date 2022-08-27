; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@0 = private unnamed_addr constant [19 x i8] c"Thread 1 finished\0A\00", align 1
@1 = private unnamed_addr constant [19 x i8] c"Thread 2 finished\0A\00", align 1
@2 = private unnamed_addr constant [19 x i8] c"Thread 3 finished\0A\00", align 1
@3 = private unnamed_addr constant [18 x i8] c"Program finished\0A\00", align 1

declare i32 @usleep(i32)

define i32 @main() {
entry.l3:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %t1 = alloca ptr, align 8
  store ptr null, ptr %t1, align 8
  %t2 = alloca ptr, align 8
  store ptr null, ptr %t2, align 8
  %t3 = alloca ptr, align 8
  store ptr null, ptr %t3, align 8
  %0 = alloca {}, align 8
  %1 = alloca ptr, align 8
  %2 = call i32 @pthread_create(ptr %1, ptr null, ptr @_thread0, ptr %0)
  %3 = load ptr, ptr %1, align 8
  store ptr %3, ptr %t1, align 8
  %4 = alloca { ptr, ptr }, align 8
  %5 = getelementptr inbounds { ptr, ptr }, ptr %4, i32 0, i32 0
  store ptr %t1, ptr %5, align 8
  %6 = getelementptr inbounds { ptr, ptr }, ptr %4, i32 0, i32 1
  store ptr %t3, ptr %6, align 8
  %7 = alloca ptr, align 8
  %8 = call i32 @pthread_create(ptr %7, ptr null, ptr @_thread1, ptr %4)
  %9 = load ptr, ptr %7, align 8
  store ptr %9, ptr %t2, align 8
  %10 = alloca {}, align 8
  %11 = alloca ptr, align 8
  %12 = call i32 @pthread_create(ptr %11, ptr null, ptr @_thread2, ptr %10)
  %13 = load ptr, ptr %11, align 8
  store ptr %13, ptr %t3, align 8
  %14 = load ptr, ptr %t2, align 8
  %15 = call i32 @pthread_join(ptr %14, ptr null)
  %16 = alloca i32, align 4
  store i32 1, ptr %16, align 4
  %17 = call i32 (ptr, ...) @printf(ptr @3)
  %18 = load i32, ptr %result, align 4
  ret i32 %18
}

define internal ptr @_thread0(ptr %0) {
thread.entry.l8:
  %1 = alloca i32, align 4
  store i32 300000, ptr %1, align 4
  %2 = load i32, ptr %1, align 4
  %3 = call i32 @usleep(i32 %2)
  %4 = alloca i32, align 4
  store i32 %3, ptr %4, align 4
  %5 = call i32 (ptr, ...) @printf(ptr @0)
  ret ptr null
}

declare i32 @printf(ptr, ...)

declare i32 @pthread_create(ptr, ptr, ptr, ptr)

define internal ptr @_thread1(ptr %0) {
thread.entry.l13:
  %1 = getelementptr inbounds { ptr, ptr }, ptr %0, i32 0, i32 0
  %2 = load ptr, ptr %1, align 8
  %3 = getelementptr inbounds { ptr, ptr }, ptr %0, i32 0, i32 1
  %4 = load ptr, ptr %3, align 8
  %5 = load ptr, ptr %2, align 8
  %6 = call i32 @pthread_join(ptr %5, ptr null)
  %7 = load ptr, ptr %4, align 8
  %8 = call i32 @pthread_join(ptr %7, ptr null)
  %9 = alloca i32, align 4
  store i32 2, ptr %9, align 4
  %10 = call i32 (ptr, ...) @printf(ptr @1)
  ret ptr null
}

declare i32 @pthread_join(ptr, ptr)

define internal ptr @_thread2(ptr %0) {
thread.entry.l18:
  %1 = alloca i32, align 4
  store i32 200000, ptr %1, align 4
  %2 = load i32, ptr %1, align 4
  %3 = call i32 @usleep(i32 %2)
  %4 = alloca i32, align 4
  store i32 %3, ptr %4, align 4
  %5 = call i32 (ptr, ...) @printf(ptr @2)
  ret ptr null
}
