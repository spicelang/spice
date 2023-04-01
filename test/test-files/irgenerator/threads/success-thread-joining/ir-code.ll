; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [19 x i8] c"Thread 1 finished\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [19 x i8] c"Thread 2 finished\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [19 x i8] c"Thread 3 finished\0A\00", align 1
@printf.str.3 = private unnamed_addr constant [18 x i8] c"Program finished\0A\00", align 1

declare void @usleep(i32)

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %t1 = alloca ptr, align 8
  %t2 = alloca ptr, align 8
  %t3 = alloca ptr, align 8
  %1 = alloca {}, align 8
  %2 = alloca { ptr, ptr }, align 8
  %3 = alloca {}, align 8
  store i32 0, ptr %result, align 4
  store ptr null, ptr %t1, align 8
  store ptr null, ptr %t2, align 8
  store ptr null, ptr %t3, align 8
  %4 = alloca ptr, align 8
  %5 = call i32 @pthread_create(ptr %4, ptr null, ptr @_thread0, ptr %1)
  %6 = load ptr, ptr %4, align 8
  store ptr %6, ptr %t1, align 8
  %7 = getelementptr inbounds { ptr, ptr }, ptr %2, i32 0, i32 0
  store ptr %t3, ptr %7, align 8
  %8 = getelementptr inbounds { ptr, ptr }, ptr %2, i32 0, i32 1
  store ptr %t1, ptr %8, align 8
  %9 = alloca ptr, align 8
  %10 = call i32 @pthread_create(ptr %9, ptr null, ptr @_thread1, ptr %2)
  %11 = load ptr, ptr %9, align 8
  store ptr %11, ptr %t2, align 8
  %12 = alloca ptr, align 8
  %13 = call i32 @pthread_create(ptr %12, ptr null, ptr @_thread2, ptr %3)
  %14 = load ptr, ptr %12, align 8
  store ptr %14, ptr %t3, align 8
  %15 = load ptr, ptr %t2, align 8
  %16 = call i32 @pthread_join(ptr %15, ptr null)
  %17 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.3)
  %18 = load i32, ptr %result, align 4
  ret i32 %18
}

define private ptr @_thread0(ptr %0) {
  call void @usleep(i32 300000)
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret ptr null
}

declare i32 @printf(ptr noundef, ...)

declare i32 @pthread_create(ptr, ptr, ptr, ptr)

define private ptr @_thread1(ptr %0) {
  %2 = getelementptr inbounds { ptr, ptr }, ptr %0, i32 0, i32 0
  %3 = load ptr, ptr %2, align 8
  %4 = getelementptr inbounds { ptr, ptr }, ptr %0, i32 0, i32 1
  %5 = load ptr, ptr %4, align 8
  %6 = load ptr, ptr %5, align 8
  %7 = call i32 @pthread_join(ptr %6, ptr null)
  %8 = load ptr, ptr %3, align 8
  %9 = call i32 @pthread_join(ptr %8, ptr null)
  %10 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1)
  ret ptr null
}

declare i32 @pthread_join(ptr, ptr)

define private ptr @_thread2(ptr %0) {
  call void @usleep(i32 200000)
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2)
  ret ptr null
}

attributes #0 = { noinline nounwind optnone uwtable }
