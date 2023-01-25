; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [25 x i8] c"Starting one thread ...\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [25 x i8] c"Hello from the thread 1\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [25 x i8] c"Hello from the thread 2\0A\00", align 1
@printf.str.3 = private unnamed_addr constant [21 x i8] c"Hello from original\0A\00", align 1

declare i32 @usleep(i32)

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %1 = alloca {}, align 8
  %2 = alloca {}, align 8
  store i32 0, ptr %result, align 4
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %4 = alloca ptr, align 8
  %5 = call i32 @pthread_create(ptr %4, ptr null, ptr @_thread0, ptr %1)
  %6 = alloca ptr, align 8
  %7 = call i32 @pthread_create(ptr %6, ptr null, ptr @_thread1, ptr %2)
  %8 = call i32 @usleep(i32 1000000)
  %9 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.3)
  %10 = load i32, ptr %result, align 4
  ret i32 %10
}

declare i32 @printf(ptr noundef, ...)

define private ptr @_thread0(ptr %0) {
  %2 = call i32 @usleep(i32 500000)
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1)
  ret ptr null
}

declare i32 @pthread_create(ptr, ptr, ptr, ptr)

define private ptr @_thread1(ptr %0) {
  %2 = call i32 @usleep(i32 200000)
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2)
  ret ptr null
}

attributes #0 = { noinline nounwind optnone uwtable }
