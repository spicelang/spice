; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__Socket__int_short_NestedSocket = type { i32, i16, %_s__NestedSocket__string_long }
%_s__NestedSocket__string_long = type { ptr, i64 }

@0 = private unnamed_addr constant [17 x i8] c"Test string: %s\0A\00", align 1
@1 = private unnamed_addr constant [12 x i8] c"Socket: %d\0A\00", align 1

define i32 @main() {
entry.l3:
  %result = alloca i32, align 4
  %s = alloca i16, align 2
  %0 = alloca %_s__Socket__int_short_NestedSocket, align 8
  store i32 0, ptr %result, align 4
  store i16 8080, ptr %s, align 2
  %1 = load i16, ptr %s, align 2
  %2 = call %_s__Socket__int_short_NestedSocket @_f__void__openServerSocket__short(i16 %1)
  store %_s__Socket__int_short_NestedSocket %2, ptr %0, align 8
  %3 = getelementptr inbounds %_s__Socket__int_short_NestedSocket, ptr %0, i32 0, i32 2
  %4 = getelementptr inbounds %_s__NestedSocket__string_long, ptr %3, i32 0, i32 0
  %5 = load ptr, ptr %4, align 8
  %6 = call i32 (ptr, ...) @printf(ptr @0, ptr %5)
  %7 = getelementptr inbounds %_s__Socket__int_short_NestedSocket, ptr %0, i32 0, i32 0
  %8 = load i32, ptr %7, align 4
  %9 = call i32 (ptr, ...) @printf(ptr @1, i32 %8)
  %10 = load i32, ptr %result, align 4
  ret i32 %10
}

declare %_s__Socket__int_short_NestedSocket @_f__void__openServerSocket__short(i16)

declare i32 @printf(ptr, ...)
