; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.Stack = type { ptr, i64, i64 }
%struct.Optional = type { %struct.Stack, i1 }
%struct.Optional.0 = type { %struct.String, i1 }
%struct.String = type { ptr, i64, i64 }

@printf.str.3 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@anon.string.0 = private unnamed_addr constant [15 x i8] c"This is a test\00", align 1
@anon.string.1 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'oi2.isPresent()' evaluated to false.\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %doubleStack = alloca %struct.Stack, align 8
  %1 = alloca double, align 8
  %oi = alloca %struct.Optional, align 8
  %oi2 = alloca %struct.Optional.0, align 8
  %2 = alloca %struct.String, align 8
  call void @_ZN5StackIdE4ctorEv(ptr nonnull %doubleStack) #3
  store double 4.566000e+00, ptr %1, align 8
  call void @_ZN5StackIdE4pushERKd(ptr nonnull %doubleStack, ptr nonnull %1) #3
  call void @"_ZN8OptionalI20std/data/stack.StackIdEE4ctorEv"(ptr nonnull %oi) #3
  %3 = call i1 @"_ZN8OptionalI20std/data/stack.StackIdEE9isPresentEv"(ptr nonnull %oi) #3
  %4 = zext i1 %3 to i32
  %5 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.3, i32 %4)
  call void @"_ZN8OptionalI20std/data/stack.StackIdEE3setERK20std/data/stack.StackIdE"(ptr nonnull %oi, ptr nonnull %doubleStack) #3
  %6 = call i1 @"_ZN8OptionalI20std/data/stack.StackIdEE9isPresentEv"(ptr nonnull %oi) #3
  %7 = zext i1 %6 to i32
  %8 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.3, i32 %7)
  %9 = call ptr @"_ZN8OptionalI20std/data/stack.StackIdEE3getEv"(ptr nonnull %oi) #3
  %10 = call i64 @_ZN5StackIdE7getSizeEv(ptr %9) #3
  %11 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.3, i64 %10)
  call void @"_ZN8OptionalI20std/data/stack.StackIdEE5clearEv"(ptr nonnull %oi) #3
  %12 = call i1 @"_ZN8OptionalI20std/data/stack.StackIdEE9isPresentEv"(ptr nonnull %oi) #3
  %13 = zext i1 %12 to i32
  %14 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.3, i32 %13)
  call void @_ZN6String4ctorEPc(ptr nonnull %2, ptr nonnull @anon.string.0) #3
  call void @_ZN8OptionalI18__rt_string.StringE4ctorERK18__rt_string.String(ptr nonnull %oi2, ptr nonnull %2) #3
  %15 = call i1 @_ZN8OptionalI18__rt_string.StringE9isPresentEv(ptr nonnull %oi2) #3
  br i1 %15, label %assert.exit.L18, label %assert.then.L18, !prof !0

assert.then.L18:                                  ; preds = %0
  %16 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L18:                                  ; preds = %0
  call void @_ZN6String4dtorEv(ptr nonnull %2) #3
  call void @_ZN5StackIdE4dtorEv(ptr nonnull %doubleStack) #3
  ret i32 0
}

declare void @_ZN5StackIdE4ctorEv(ptr) local_unnamed_addr

declare void @_ZN5StackIdE4pushERKd(ptr, ptr) local_unnamed_addr

declare void @"_ZN8OptionalI20std/data/stack.StackIdEE4ctorEv"(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare i1 @"_ZN8OptionalI20std/data/stack.StackIdEE9isPresentEv"(ptr) local_unnamed_addr

declare void @"_ZN8OptionalI20std/data/stack.StackIdEE3setERK20std/data/stack.StackIdE"(ptr, ptr) local_unnamed_addr

declare ptr @"_ZN8OptionalI20std/data/stack.StackIdEE3getEv"(ptr) local_unnamed_addr

declare i64 @_ZN5StackIdE7getSizeEv(ptr) local_unnamed_addr

declare void @"_ZN8OptionalI20std/data/stack.StackIdEE5clearEv"(ptr) local_unnamed_addr

declare void @_ZN6String4ctorEPc(ptr, ptr) local_unnamed_addr

declare void @_ZN8OptionalI18__rt_string.StringE4ctorERK18__rt_string.String(ptr, ptr) local_unnamed_addr

declare i1 @_ZN8OptionalI18__rt_string.StringE9isPresentEv(ptr) local_unnamed_addr

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) local_unnamed_addr #2

declare void @_ZN6String4dtorEv(ptr) local_unnamed_addr

declare void @_ZN5StackIdE4dtorEv(ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { cold noreturn nounwind }
attributes #3 = { nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
