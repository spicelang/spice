; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anon.string.0 = private unnamed_addr constant [5 x i8] c"5.67\00", align 1
@anon.string.1 = private unnamed_addr constant [6 x i8] c"-6546\00", align 1
@anon.string.2 = private unnamed_addr constant [3 x i8] c"56\00", align 1
@anon.string.3 = private unnamed_addr constant [4 x i8] c"-12\00", align 1
@anon.string.4 = private unnamed_addr constant [3 x i8] c"12\00", align 1
@anon.string.5 = private unnamed_addr constant [2 x i8] c"i\00", align 1
@anon.string.6 = private unnamed_addr constant [5 x i8] c"true\00", align 1
@anon.string.7 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'asBool1 == true' evaluated to false.\00", align 1
@anon.string.8 = private unnamed_addr constant [6 x i8] c"false\00", align 1
@anon.string.9 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'asBool2 == false' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [25 x i8] c"All assertions succeeded\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = tail call double @_Z8toDoublePc(ptr nonnull @anon.string.0) #3
  %2 = tail call i32 @_Z5toIntPc(ptr nonnull @anon.string.1) #3
  %3 = tail call i64 @_Z6toLongPc(ptr nonnull @anon.string.2) #3
  %4 = tail call i16 @_Z7toShortPc(ptr nonnull @anon.string.3) #3
  %5 = tail call i8 @_Z6toBytePc(ptr nonnull @anon.string.4) #3
  %6 = tail call i8 @_Z6toCharPc(ptr nonnull @anon.string.5) #3
  %7 = tail call i1 @_Z6toBoolPc(ptr nonnull @anon.string.6) #3
  br i1 %7, label %assert.exit.L35, label %assert.then.L35, !prof !0

assert.then.L35:                                  ; preds = %0
  %8 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.7)
  tail call void @exit(i32 1)
  unreachable

assert.exit.L35:                                  ; preds = %0
  %9 = tail call i1 @_Z6toBoolPc(ptr nonnull @anon.string.8) #3
  br i1 %9, label %assert.then.L37, label %assert.exit.L37, !prof !1

assert.then.L37:                                  ; preds = %assert.exit.L35
  %10 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.9)
  tail call void @exit(i32 1)
  unreachable

assert.exit.L37:                                  ; preds = %assert.exit.L35
  %11 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0)
  ret i32 0
}

declare double @_Z8toDoublePc(ptr) local_unnamed_addr

declare i32 @_Z5toIntPc(ptr) local_unnamed_addr

declare i64 @_Z6toLongPc(ptr) local_unnamed_addr

declare i16 @_Z7toShortPc(ptr) local_unnamed_addr

declare i8 @_Z6toBytePc(ptr) local_unnamed_addr

declare i8 @_Z6toCharPc(ptr) local_unnamed_addr

declare i1 @_Z6toBoolPc(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) local_unnamed_addr #2

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { cold noreturn nounwind }
attributes #3 = { nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
!1 = !{!"branch_weights", i32 1, i32 2000}
