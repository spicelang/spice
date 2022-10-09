; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__Stamp__double_bool = type { double, i1 }
%_s__Letter__string_Stamp = type { ptr, %_s__Stamp__double_bool }

@0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@1 = private unnamed_addr constant [17 x i8] c"Stamp glued: %d\0A\00", align 1

define internal %_s__Stamp__double_bool @_mf__Letter__getStamp(ptr %0) {
entry.l23:
  %this = alloca ptr, align 8
  %result = alloca %_s__Stamp__double_bool, align 8
  store ptr %0, ptr %this, align 8
  %1 = load ptr, ptr %this, align 8
  %2 = getelementptr inbounds %_s__Letter__string_Stamp, ptr %1, i32 0, i32 1
  %3 = load %_s__Stamp__double_bool, ptr %2, align 8
  ret %_s__Stamp__double_bool %3
}

define i32 @main() {
entry.l31:
  %result = alloca i32, align 4
  %0 = alloca %_s__Letter__string_Stamp, align 8
  %1 = alloca %_s__Stamp__double_bool, align 8
  %2 = alloca %_s__Stamp__double_bool, align 8
  store i32 0, ptr %result, align 4
  %3 = getelementptr inbounds %_s__Letter__string_Stamp, ptr %0, i32 0, i32 0
  store ptr @0, ptr %3, align 8
  %4 = getelementptr inbounds %_s__Stamp__double_bool, ptr %1, i32 0, i32 0
  store double 3.400000e+00, ptr %4, align 8
  %5 = getelementptr inbounds %_s__Stamp__double_bool, ptr %1, i32 0, i32 1
  store i1 true, ptr %5, align 1
  %6 = load %_s__Stamp__double_bool, ptr %1, align 8
  %7 = getelementptr inbounds %_s__Letter__string_Stamp, ptr %0, i32 0, i32 1
  store %_s__Stamp__double_bool %6, ptr %7, align 8
  %8 = call %_s__Stamp__double_bool @_mf__Letter__getStamp(ptr %0)
  store %_s__Stamp__double_bool %8, ptr %2, align 8
  %9 = getelementptr inbounds %_s__Stamp__double_bool, ptr %2, i32 0, i32 1
  %10 = load i1, ptr %9, align 1
  %11 = zext i1 %10 to i32
  %12 = call i32 (ptr, ...) @printf(ptr @1, i32 %11)
  %13 = load i32, ptr %result, align 4
  ret i32 %13
}

declare i32 @printf(ptr, ...)
