; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__Letter__string_Stamp = type { ptr, %__Stamp__double_bool }
%__Stamp__double_bool = type { double, i1 }

@printf.str.0 = private unnamed_addr constant [21 x i8] c"Value: %f, glued: %d\00", align 1
@anon.string.0 = private unnamed_addr constant [17 x i8] c"This is a letter\00", align 1
@anon.struct.0 = private unnamed_addr constant %__Letter__string_Stamp { ptr @anon.string.0, %__Stamp__double_bool { double 3.400000e+00, i1 true } }
@printf.str.1 = private unnamed_addr constant [17 x i8] c"Stamp glued: %d\0A\00", align 1

define private void @_mp__Stamp__void__print(ptr noundef nonnull %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %value_addr = getelementptr inbounds %__Stamp__double_bool, ptr %2, i32 0, i32 0
  %3 = load double, ptr %value_addr, align 8
  %4 = load ptr, ptr %this, align 8
  %glued_addr = getelementptr inbounds %__Stamp__double_bool, ptr %4, i32 0, i32 1
  %5 = load i1, ptr %glued_addr, align 1
  %6 = zext i1 %5 to i32
  %7 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, double %3, i32 %6)
  ret void
}

declare i32 @printf(ptr noundef, ...)

define private %__Stamp__double_bool @_mf__Letter__Stamp__getStamp(ptr noundef nonnull %0) {
  %result = alloca %__Stamp__double_bool, align 8
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %stamp_addr = getelementptr inbounds %__Letter__string_Stamp, ptr %2, i32 0, i32 1
  %3 = load %__Stamp__double_bool, ptr %stamp_addr, align 8
  ret %__Stamp__double_bool %3
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %letter = alloca %__Letter__string_Stamp, align 8
  %stamp = alloca %__Stamp__double_bool, align 8
  store i32 0, ptr %result, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %letter, ptr @anon.struct.0, i64 24, i1 false)
  store %__Letter__string_Stamp { ptr @anon.string.0, %__Stamp__double_bool { double 3.400000e+00, i1 true } }, ptr %letter, align 8
  %stamp_addr = getelementptr inbounds %__Letter__string_Stamp, ptr %letter, i32 0, i32 1
  %glued_addr = getelementptr inbounds %__Stamp__double_bool, ptr %stamp_addr, i32 0, i32 1
  %1 = load i1, ptr %glued_addr, align 1
  %2 = zext i1 %1 to i32
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %2)
  %4 = call %__Stamp__double_bool @_mf__Letter__Stamp__getStamp(ptr %letter)
  store %__Stamp__double_bool %4, ptr %stamp, align 8
  store %__Stamp__double_bool %4, ptr %stamp, align 8
  call void @_mp__Stamp__void__print(ptr %stamp)
  %5 = load i32, ptr %result, align 4
  ret i32 %5
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
