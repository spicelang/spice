; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anon.string.0 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'asDouble == 100.0' evaluated to false.\00", align 1
@anon.string.1 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition 'asInt == 84' evaluated to false.\00", align 1
@anon.string.2 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'asShort == 106s' evaluated to false.\00", align 1
@anon.string.3 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition 'asLong == 75l' evaluated to false.\00", align 1
@anon.string.4 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'asBool1 == true' evaluated to false.\00", align 1
@anon.string.5 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'asBool2 == false' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [25 x i8] c"All assertions succeeded\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = tail call double @_Z8toDoubleh(i8 100) #3
  %2 = fcmp oeq double %1, 1.000000e+02
  br i1 %2, label %assert.exit.L6, label %assert.then.L6, !prof !0

assert.then.L6:                                   ; preds = %0
  %3 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.0)
  tail call void @llvm.trap()
  unreachable

assert.exit.L6:                                   ; preds = %0
  %4 = tail call i32 @_Z5toInth(i8 84) #3
  %5 = icmp eq i32 %4, 84
  br i1 %5, label %assert.exit.L10, label %assert.then.L10, !prof !0

assert.then.L10:                                  ; preds = %assert.exit.L6
  %6 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.1)
  tail call void @llvm.trap()
  unreachable

assert.exit.L10:                                  ; preds = %assert.exit.L6
  %7 = tail call i16 @_Z7toShorth(i8 106) #3
  %8 = icmp eq i16 %7, 106
  br i1 %8, label %assert.exit.L14, label %assert.then.L14, !prof !0

assert.then.L14:                                  ; preds = %assert.exit.L10
  %9 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.2)
  tail call void @llvm.trap()
  unreachable

assert.exit.L14:                                  ; preds = %assert.exit.L10
  %10 = tail call i64 @_Z6toLongh(i8 75) #3
  %11 = icmp eq i64 %10, 75
  br i1 %11, label %assert.exit.L18, label %assert.then.L18, !prof !0

assert.then.L18:                                  ; preds = %assert.exit.L14
  %12 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.3)
  tail call void @llvm.trap()
  unreachable

assert.exit.L18:                                  ; preds = %assert.exit.L14
  %13 = tail call i1 @_Z6toBoolh(i8 49) #3
  br i1 %13, label %assert.exit.L26, label %assert.then.L26, !prof !0

assert.then.L26:                                  ; preds = %assert.exit.L18
  %14 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.4)
  tail call void @llvm.trap()
  unreachable

assert.exit.L26:                                  ; preds = %assert.exit.L18
  %15 = tail call i1 @_Z6toBoolh(i8 48) #3
  br i1 %15, label %assert.then.L28, label %assert.exit.L28, !prof !1

assert.then.L28:                                  ; preds = %assert.exit.L26
  %16 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.5)
  tail call void @llvm.trap()
  unreachable

assert.exit.L28:                                  ; preds = %assert.exit.L26
  %17 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0)
  ret i32 0
}

declare double @_Z8toDoubleh(i8) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

; Function Attrs: cold noreturn nounwind
declare void @llvm.trap() #2

declare i32 @_Z5toInth(i8) local_unnamed_addr

declare i16 @_Z7toShorth(i8) local_unnamed_addr

declare i64 @_Z6toLongh(i8) local_unnamed_addr

declare i1 @_Z6toBoolh(i8) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { cold noreturn nounwind }
attributes #3 = { nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
!1 = !{!"branch_weights", i32 1, i32 2000}
