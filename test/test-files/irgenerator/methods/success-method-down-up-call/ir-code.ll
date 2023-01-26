; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__char__TestStruct__char_int = type { i8, i32 }

@anon.struct.0 = private unnamed_addr constant %__char__TestStruct__char_int { i8 97, i32 1 }
@printf.str.0 = private unnamed_addr constant [10 x i8] c"Test: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %s = alloca %__char__TestStruct__char_int, align 8
  store i32 0, ptr %result, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %s, ptr @anon.struct.0, i64 8, i1 false)
  call void @_mp__TestStruct_char__void__printTest(ptr %s)
  %1 = load i32, ptr %result, align 4
  ret i32 %1
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

define private void @_mp__TestStruct_char__void__printTest(ptr noundef nonnull %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %3 = call i32 @_mf__TestStruct_char__int__getTest(ptr %2)
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %3)
  ret void
}

declare i32 @printf(ptr noundef, ...)

define private i32 @_mf__TestStruct_char__int__getTest(ptr noundef nonnull %0) {
  %result = alloca i32, align 4
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %test = getelementptr inbounds %__char__TestStruct__char_int, ptr %2, i32 0, i32 1
  %3 = load i32, ptr %test, align 4
  %4 = icmp eq i32 %3, 1
  br i1 %4, label %if.then.L18, label %if.exit.L18

if.then.L18:                                      ; preds = %1
  %5 = load ptr, ptr %this, align 8
  %test1 = getelementptr inbounds %__char__TestStruct__char_int, ptr %5, i32 0, i32 1
  %6 = load i32, ptr %test1, align 4
  %7 = add i32 %6, 1
  store i32 %7, ptr %test1, align 4
  %8 = load ptr, ptr %this, align 8
  call void @_mp__TestStruct_char__void__printTest(ptr %8)
  br label %if.exit.L18

if.exit.L18:                                      ; preds = %if.then.L18, %1
  %9 = load ptr, ptr %this, align 8
  %test2 = getelementptr inbounds %__char__TestStruct__char_int, ptr %9, i32 0, i32 1
  %10 = load i32, ptr %test2, align 4
  ret i32 %10
}

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
