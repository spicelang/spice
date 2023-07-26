; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.Timer = type { i64, i64, i32, ptr }

@printf.str.0 = private unnamed_addr constant [28 x i8] c"Check for %d +/-%d, got %d\0A\00", align 1
@anon.string.1 = private unnamed_addr constant [85 x i8] c"Assertion failed: Condition 'isInRange(t.getDuration(), 10l, 3)' evaluated to false.\00", align 1
@anon.string.2 = private unnamed_addr constant [72 x i8] c"Assertion failed: Condition 't.getDuration() == 0l' evaluated to false.\00", align 1
@anon.string.3 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition 'duration == 0l' evaluated to false.\00", align 1
@anon.string.4 = private unnamed_addr constant [84 x i8] c"Assertion failed: Condition 'isInRange(duration, 20000l, 5000)' evaluated to false.\00", align 1
@printf.str.1 = private unnamed_addr constant [22 x i8] c"All assertions passed\00", align 1

; Function Attrs: nofree nounwind
define private fastcc i1 @_Z9isInRangemmj(i64 %0, i64 %1, i32 %2) unnamed_addr #0 {
land.exit.L6C12:
  %3 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i64 %1, i32 %2, i64 %0)
  %4 = zext i32 %2 to i64
  %5 = sub i64 %1, %4
  %.not = icmp ule i64 %5, %0
  %6 = add i64 %4, %1
  %7 = icmp uge i64 %6, %0
  %land_phi = and i1 %.not, %7
  ret i1 %land_phi
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #1 {
  %t = alloca %struct.Timer, align 8
  %duration = alloca i64, align 8
  %1 = alloca %struct.Timer, align 8
  call void @_ZN5Timer4ctorEv(ptr nonnull %t) #4
  %2 = call i64 @_ZN5Timer11getDurationEv(ptr nonnull %t) #4
  %3 = icmp eq i64 %2, 0
  br i1 %3, label %assert.exit.L12, label %assert.then.L12, !prof !0

assert.then.L12:                                  ; preds = %0
  %4 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.2)
  call void @llvm.trap()
  unreachable

assert.exit.L12:                                  ; preds = %0
  call void @_ZN5Timer5startEv(ptr nonnull %t) #4
  call void @_Z5delayi(i32 10) #4
  call void @_ZN5Timer4stopEv(ptr nonnull %t) #4
  %5 = call i64 @_ZN5Timer11getDurationEv(ptr nonnull %t) #4
  %6 = call fastcc i1 @_Z9isInRangemmj(i64 %5, i64 10, i32 3) #4
  br i1 %6, label %assert.exit.L16, label %assert.then.L16, !prof !0

assert.then.L16:                                  ; preds = %assert.exit.L12
  %7 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.1)
  call void @llvm.trap()
  unreachable

assert.exit.L16:                                  ; preds = %assert.exit.L12
  store i64 0, ptr %duration, align 8
  call void @_ZN5Timer4ctorEiPm(ptr nonnull %1, i32 0, ptr nonnull %duration) #4
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(32) %t, ptr noundef nonnull align 8 dereferenceable(32) %1, i64 32, i1 false)
  %8 = call i64 @_ZN5Timer11getDurationEv(ptr nonnull %t) #4
  %9 = icmp eq i64 %8, 0
  br i1 %9, label %assert.exit.L21, label %assert.then.L21, !prof !0

assert.then.L21:                                  ; preds = %assert.exit.L16
  %10 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.2)
  call void @llvm.trap()
  unreachable

assert.exit.L21:                                  ; preds = %assert.exit.L16
  %11 = load i64, ptr %duration, align 8
  %12 = icmp eq i64 %11, 0
  br i1 %12, label %assert.exit.L22, label %assert.then.L22, !prof !0

assert.then.L22:                                  ; preds = %assert.exit.L21
  %13 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.3)
  call void @llvm.trap()
  unreachable

assert.exit.L22:                                  ; preds = %assert.exit.L21
  call void @_ZN5Timer5startEv(ptr nonnull %t) #4
  call void @_Z5delayi(i32 10) #4
  call void @_ZN5Timer5pauseEv(ptr nonnull %t) #4
  call void @_Z5delayi(i32 100) #4
  call void @_ZN5Timer6resumeEv(ptr nonnull %t) #4
  call void @_Z5delayi(i32 10) #4
  call void @_ZN5Timer4stopEv(ptr nonnull %t) #4
  %14 = load i64, ptr %duration, align 8
  %15 = call fastcc i1 @_Z9isInRangemmj(i64 %14, i64 20000, i32 5000) #4
  br i1 %15, label %assert.exit.L30, label %assert.then.L30, !prof !0

assert.then.L30:                                  ; preds = %assert.exit.L22
  %16 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.4)
  call void @llvm.trap()
  unreachable

assert.exit.L30:                                  ; preds = %assert.exit.L22
  %17 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1)
  ret i32 0
}

declare void @_ZN5Timer4ctorEv(ptr) local_unnamed_addr

declare i64 @_ZN5Timer11getDurationEv(ptr) local_unnamed_addr

; Function Attrs: cold noreturn nounwind
declare void @llvm.trap() #2

declare void @_ZN5Timer5startEv(ptr) local_unnamed_addr

declare void @_Z5delayi(i32) local_unnamed_addr

declare void @_ZN5Timer4stopEv(ptr) local_unnamed_addr

declare void @_ZN5Timer4ctorEiPm(ptr, i32, ptr) local_unnamed_addr

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #3

declare void @_ZN5Timer5pauseEv(ptr) local_unnamed_addr

declare void @_ZN5Timer6resumeEv(ptr) local_unnamed_addr

attributes #0 = { nofree nounwind }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { cold noreturn nounwind }
attributes #3 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #4 = { nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
