; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__Vector__bool_string = type { i1, ptr }

@0 = private unnamed_addr constant [12 x i8] c"Test string\00", align 1
@1 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@2 = private unnamed_addr constant [16 x i8] c"Fields: %d, %s\0A\00", align 1
@3 = private unnamed_addr constant [16 x i8] c"Another message\00", align 1
@4 = private unnamed_addr constant [16 x i8] c"Fields: %d, %s\0A\00", align 1
@5 = private unnamed_addr constant [13 x i8] c"Message: %s\0A\00", align 1

define internal void @_mp__Vector__void__ctor(ptr %0) {
entry.l6:
  %this = alloca ptr, align 8
  %msg = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr @0, ptr %msg, align 8
  %1 = load ptr, ptr %this, align 8
  %2 = getelementptr inbounds %_s__Vector__bool_string, ptr %1, i32 0, i32 0
  store i1 false, ptr %2, align 1
  %3 = load ptr, ptr %msg, align 8
  %4 = load ptr, ptr %this, align 8
  %5 = getelementptr inbounds %_s__Vector__bool_string, ptr %4, i32 0, i32 1
  store ptr %3, ptr %5, align 8
  ret void
}

define internal void @_mp__Vector__void__ctor__string(ptr %0, ptr %1) {
entry.l6:
  %this = alloca ptr, align 8
  %msg = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %msg, align 8
  %2 = load ptr, ptr %this, align 8
  %3 = getelementptr inbounds %_s__Vector__bool_string, ptr %2, i32 0, i32 0
  store i1 false, ptr %3, align 1
  %4 = load ptr, ptr %msg, align 8
  %5 = load ptr, ptr %this, align 8
  %6 = getelementptr inbounds %_s__Vector__bool_string, ptr %5, i32 0, i32 1
  store ptr %4, ptr %6, align 8
  ret void
}

define internal ptr @_mf__Vector__string__test(ptr %0) {
entry.l11:
  %this = alloca ptr, align 8
  %result = alloca ptr, align 8
  %1 = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr @1, ptr %1, align 8
  %2 = load ptr, ptr %1, align 8
  ret ptr %2
}

define i32 @main() {
entry.l15:
  %result = alloca i32, align 4
  %vec = alloca %_s__Vector__bool_string, align 8
  %vec1 = alloca ptr, align 8
  %0 = alloca %_s__Vector__bool_string, align 8
  %1 = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  call void @_mp__Vector__void__ctor(ptr %vec)
  %2 = getelementptr inbounds %_s__Vector__bool_string, ptr %vec, i32 0, i32 0
  %3 = load i1, ptr %2, align 1
  %4 = zext i1 %3 to i32
  %5 = getelementptr inbounds %_s__Vector__bool_string, ptr %vec, i32 0, i32 1
  %6 = load ptr, ptr %5, align 8
  %7 = call i32 (ptr, ...) @printf(ptr @2, i32 %4, ptr %6)
  store ptr @3, ptr %vec1, align 8
  %8 = load ptr, ptr %vec1, align 8
  call void @_mp__Vector__void__ctor__string(ptr %vec, ptr %8)
  %9 = load %_s__Vector__bool_string, ptr %vec, align 8
  store %_s__Vector__bool_string %9, ptr %vec, align 8
  %10 = getelementptr inbounds %_s__Vector__bool_string, ptr %vec, i32 0, i32 0
  %11 = load i1, ptr %10, align 1
  %12 = zext i1 %11 to i32
  %13 = getelementptr inbounds %_s__Vector__bool_string, ptr %vec, i32 0, i32 1
  %14 = load ptr, ptr %13, align 8
  %15 = call i32 (ptr, ...) @printf(ptr @4, i32 %12, ptr %14)
  call void @_mp__Vector__void__ctor(ptr %0)
  %16 = call ptr @_mf__Vector__string__test(ptr %0)
  store ptr %16, ptr %1, align 8
  %17 = load ptr, ptr %1, align 8
  %18 = call i32 (ptr, ...) @printf(ptr @5, ptr %17)
  %19 = load i32, ptr %result, align 4
  ret i32 %19
}

declare i32 @printf(ptr, ...)
