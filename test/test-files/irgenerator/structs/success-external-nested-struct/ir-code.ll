; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__Socket__int_short_NestedSocket = type { i32, i16, %__NestedSocket__string_long }
%__NestedSocket__string_long = type { ptr, i64 }

@printf.str.0 = private unnamed_addr constant [17 x i8] c"Test string: %s\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [12 x i8] c"Socket: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %s = alloca %__Socket__int_short_NestedSocket, align 8
  store i32 0, ptr %result, align 4
  %1 = call %__Socket__int_short_NestedSocket @_f__void__Socket__openServerSocket__short(i16 8080)
  store %__Socket__int_short_NestedSocket %1, ptr %s, align 8
  store %__Socket__int_short_NestedSocket %1, ptr %s, align 8
  %n = getelementptr inbounds %__Socket__int_short_NestedSocket, ptr %s, i32 0, i32 2
  %testString = getelementptr inbounds %__NestedSocket__string_long, ptr %n, i32 0, i32 0
  %2 = load ptr, ptr %testString, align 8
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr %2)
  %sock = getelementptr inbounds %__Socket__int_short_NestedSocket, ptr %s, i32 0, i32 0
  %4 = load i32, ptr %sock, align 4
  %5 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %4)
  %6 = load i32, ptr %result, align 4
  ret i32 %6
}

declare %__Socket__int_short_NestedSocket @_f__void__Socket__openServerSocket__short(i16)

declare i32 @printf(ptr noundef, ...)

attributes #0 = { noinline nounwind optnone uwtable }
