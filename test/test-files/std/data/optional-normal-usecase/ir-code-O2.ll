; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__double__Stack__doubleptr_long_long_int = type { ptr, i64, i64, i32 }
%"__std/data/stack::Stack<double>__Optional__std/data/stack::Stack<double>_bool" = type { %__double__Stack__doubleptr_long_long_int, i1 }
%"____rt_string::String__Optional____rt_string::String_bool" = type { %__String__charptr_long_long, i1 }
%__String__charptr_long_long = type { ptr, i64, i64 }

@printf.str.3 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@anon.string.0 = private unnamed_addr constant [15 x i8] c"This is a test\00", align 1
@anon.string.1 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'oi2.isPresent()' evaluated to false.\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %doubleStack = alloca %__double__Stack__doubleptr_long_long_int, align 8
  %oi = alloca %"__std/data/stack::Stack<double>__Optional__std/data/stack::Stack<double>_bool", align 8
  %res = alloca %__double__Stack__doubleptr_long_long_int, align 8
  %oi2 = alloca %"____rt_string::String__Optional____rt_string::String_bool", align 8
  %1 = alloca %__String__charptr_long_long, align 8
  call void @_mp__Stack_double__void__ctor(ptr nonnull %doubleStack) #2
  call void @_mp__Stack_double__void__push__double(ptr nonnull %doubleStack, double 4.566000e+00) #2
  call void @"_mp__Optional_std/data/stack::Stack<double>__void__ctor"(ptr nonnull %oi) #2
  %2 = call i1 @"_mf__Optional_std/data/stack::Stack<double>__bool__isPresent"(ptr nonnull %oi) #2
  %3 = zext i1 %2 to i32
  %4 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.3, i32 %3)
  call void @"_mp__Optional_std/data/stack::Stack<double>__void__set__std/data/stack::Stack<double>ref"(ptr nonnull %oi, ptr nonnull %doubleStack) #2
  %5 = call i1 @"_mf__Optional_std/data/stack::Stack<double>__bool__isPresent"(ptr nonnull %oi) #2
  %6 = zext i1 %5 to i32
  %7 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.3, i32 %6)
  %8 = call %__double__Stack__doubleptr_long_long_int @"_mf__Optional_std/data/stack::Stack<double>__std/data/stack::Stack<double>__get"(ptr nonnull %oi) #2
  %.fca.0.extract = extractvalue %__double__Stack__doubleptr_long_long_int %8, 0
  %.fca.1.extract = extractvalue %__double__Stack__doubleptr_long_long_int %8, 1
  %.fca.1.gep = getelementptr inbounds %__double__Stack__doubleptr_long_long_int, ptr %res, i64 0, i32 1
  %.fca.2.extract = extractvalue %__double__Stack__doubleptr_long_long_int %8, 2
  %.fca.2.gep = getelementptr inbounds %__double__Stack__doubleptr_long_long_int, ptr %res, i64 0, i32 2
  %.fca.3.extract = extractvalue %__double__Stack__doubleptr_long_long_int %8, 3
  %.fca.3.gep = getelementptr inbounds %__double__Stack__doubleptr_long_long_int, ptr %res, i64 0, i32 3
  store ptr %.fca.0.extract, ptr %res, align 8
  store i64 %.fca.1.extract, ptr %.fca.1.gep, align 8
  store i64 %.fca.2.extract, ptr %.fca.2.gep, align 8
  store i32 %.fca.3.extract, ptr %.fca.3.gep, align 8
  %9 = call i64 @_mf__Stack_double__long__getSize(ptr nonnull %res) #2
  %10 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.3, i64 %9)
  call void @"_mp__Optional_std/data/stack::Stack<double>__void__clear"(ptr nonnull %oi) #2
  %11 = call i1 @"_mf__Optional_std/data/stack::Stack<double>__bool__isPresent"(ptr nonnull %oi) #2
  %12 = zext i1 %11 to i32
  %13 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.3, i32 %12)
  call void @_mp__String__void__ctor__string(ptr nonnull %1, ptr nonnull @anon.string.0) #2
  call void @"_mp__Optional___rt_string::String__void__ctor____rt_string::Stringref"(ptr nonnull %oi2, ptr nonnull %1) #2
  %14 = call i1 @"_mf__Optional___rt_string::String__bool__isPresent"(ptr nonnull %oi2) #2
  br i1 %14, label %assert.exit.L18, label %assert.then.L18, !prof !0

assert.then.L18:                                  ; preds = %0
  %15 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.1)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L18:                                  ; preds = %0
  ret i32 0
}

declare void @_mp__Stack_double__void__ctor(ptr) local_unnamed_addr

declare void @_mp__Stack_double__void__push__double(ptr, double) local_unnamed_addr

declare void @"_mp__Optional_std/data/stack::Stack<double>__void__ctor"(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare i1 @"_mf__Optional_std/data/stack::Stack<double>__bool__isPresent"(ptr) local_unnamed_addr

declare void @"_mp__Optional_std/data/stack::Stack<double>__void__set__std/data/stack::Stack<double>ref"(ptr, ptr) local_unnamed_addr

declare %__double__Stack__doubleptr_long_long_int @"_mf__Optional_std/data/stack::Stack<double>__std/data/stack::Stack<double>__get"(ptr) local_unnamed_addr

declare i64 @_mf__Stack_double__long__getSize(ptr) local_unnamed_addr

declare void @"_mp__Optional_std/data/stack::Stack<double>__void__clear"(ptr) local_unnamed_addr

declare void @_mp__String__void__ctor__string(ptr, ptr) local_unnamed_addr

declare void @"_mp__Optional___rt_string::String__void__ctor____rt_string::Stringref"(ptr, ptr) local_unnamed_addr

declare i1 @"_mf__Optional___rt_string::String__bool__isPresent"(ptr) local_unnamed_addr

declare void @exit(i32) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
