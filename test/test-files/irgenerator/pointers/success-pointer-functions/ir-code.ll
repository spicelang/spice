; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__Person__string_string_int = type { ptr, ptr, i32 }

@0 = private unnamed_addr constant [5 x i8] c"Mike\00", align 1
@1 = private unnamed_addr constant [7 x i8] c"Miller\00", align 1
@2 = private unnamed_addr constant [16 x i8] c"Person: %s, %s\0A\00", align 1
@3 = private unnamed_addr constant [25 x i8] c"Age before birthday: %d\0A\00", align 1
@4 = private unnamed_addr constant [24 x i8] c"Age after birthday: %d\0A\00", align 1

define internal void @_p__void__void__birthday__Personptr(ptr %0) {
entry.l7:
  %person = alloca ptr, align 8
  %1 = alloca i32, align 4
  store ptr %0, ptr %person, align 8
  %2 = load ptr, ptr %person, align 8
  %3 = getelementptr inbounds %_s__Person__string_string_int, ptr %2, i32 0, i32 2
  %4 = load i32, ptr %3, align 4
  %5 = add i32 %4, 1
  store i32 %5, ptr %3, align 4
  store i32 %4, ptr %1, align 4
  ret void
}

define i32 @main() {
entry.l11:
  %result = alloca i32, align 4
  %0 = alloca %_s__Person__string_string_int, align 8
  %1 = alloca ptr, align 8
  %2 = alloca i1, align 1
  store i32 0, ptr %result, align 4
  %3 = getelementptr inbounds %_s__Person__string_string_int, ptr %0, i32 0, i32 0
  store ptr @0, ptr %3, align 8
  %4 = getelementptr inbounds %_s__Person__string_string_int, ptr %0, i32 0, i32 1
  store ptr @1, ptr %4, align 8
  %5 = getelementptr inbounds %_s__Person__string_string_int, ptr %0, i32 0, i32 2
  store i32 32, ptr %5, align 4
  %6 = getelementptr inbounds %_s__Person__string_string_int, ptr %0, i32 0, i32 1
  %7 = load ptr, ptr %6, align 8
  %8 = getelementptr inbounds %_s__Person__string_string_int, ptr %0, i32 0, i32 0
  %9 = load ptr, ptr %8, align 8
  %10 = call i32 (ptr, ...) @printf(ptr @2, ptr %7, ptr %9)
  %11 = getelementptr inbounds %_s__Person__string_string_int, ptr %0, i32 0, i32 2
  %12 = load i32, ptr %11, align 4
  %13 = call i32 (ptr, ...) @printf(ptr @3, i32 %12)
  store ptr %0, ptr %1, align 8
  %14 = load ptr, ptr %1, align 8
  call void @_p__void__void__birthday__Personptr(ptr %14)
  store i1 true, ptr %2, align 1
  %15 = getelementptr inbounds %_s__Person__string_string_int, ptr %0, i32 0, i32 2
  %16 = load i32, ptr %15, align 4
  %17 = call i32 (ptr, ...) @printf(ptr @4, i32 %16)
  %18 = load i32, ptr %result, align 4
  ret i32 %18
}

declare i32 @printf(ptr, ...)
