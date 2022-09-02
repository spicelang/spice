; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__Person__string_string_int = type { ptr, ptr, i32 }

@0 = private unnamed_addr constant [5 x i8] c"John\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"Doe\00", align 1
@2 = private unnamed_addr constant [15 x i8] c"John's age: %d\00", align 1

define i32 @main() {
entry.l7:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %0 = alloca %_s__Person__string_string_int, align 8
  %1 = getelementptr inbounds %_s__Person__string_string_int, ptr %0, i32 0, i32 0
  store ptr @0, ptr %1, align 8
  %2 = getelementptr inbounds %_s__Person__string_string_int, ptr %0, i32 0, i32 1
  store ptr @1, ptr %2, align 8
  %3 = getelementptr inbounds %_s__Person__string_string_int, ptr %0, i32 0, i32 2
  store i32 46, ptr %3, align 4
  %4 = getelementptr inbounds %_s__Person__string_string_int, ptr %0, i32 0, i32 2
  store i32 47, ptr %4, align 4
  %5 = getelementptr inbounds %_s__Person__string_string_int, ptr %0, i32 0, i32 2
  %6 = load i32, ptr %5, align 4
  %7 = call i32 (ptr, ...) @printf(ptr @2, i32 %6)
  %8 = load i32, ptr %result, align 4
  ret i32 %8
}

declare i32 @printf(ptr, ...)
