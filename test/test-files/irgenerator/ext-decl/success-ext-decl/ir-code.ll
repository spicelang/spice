; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@MODE_ALL_RWX = internal constant i32 511
@0 = private unnamed_addr constant [18 x i8] c"Creating dir ...\0A\00", align 1
@1 = private unnamed_addr constant [11 x i8] c"./test-dir\00", align 1
@2 = private unnamed_addr constant [18 x i8] c"Deleting dir ...\0A\00", align 1
@3 = private unnamed_addr constant [11 x i8] c"./test-dir\00", align 1
@4 = private unnamed_addr constant [6 x i8] c"Done.\00", align 1

declare i32 @mkdir(ptr, i32)

declare i32 @rmdir(ptr)

declare void @free(ptr)

define i32 @main() {
entry.l7:
  %result = alloca i32, align 4
  %0 = alloca ptr, align 8
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %5 = call i32 (ptr, ...) @printf(ptr @0)
  store ptr @1, ptr %0, align 8
  %6 = load ptr, ptr %0, align 8
  %7 = load i32, ptr @MODE_ALL_RWX, align 4
  store i32 %7, ptr %1, align 4
  %8 = load i32, ptr %1, align 4
  %9 = call i32 @mkdir(ptr %6, i32 %8)
  store i32 %9, ptr %2, align 4
  %10 = call i32 (ptr, ...) @printf(ptr @2)
  store ptr @3, ptr %3, align 8
  %11 = load ptr, ptr %3, align 8
  %12 = call i32 @rmdir(ptr %11)
  store i32 %12, ptr %4, align 4
  %13 = call i32 (ptr, ...) @printf(ptr @4)
  %14 = load i32, ptr %result, align 4
  ret i32 %14
}

declare i32 @printf(ptr, ...)