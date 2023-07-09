; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.String = type { ptr, i64, i64 }

@printf.str.2 = private unnamed_addr constant [15 x i8] c"Capacity: %d\0A\0A\00", align 1
@anon.string.0 = private unnamed_addr constant [5 x i8] c"ello\00", align 1
@printf.str.3 = private unnamed_addr constant [13 x i8] c"Content: %s\0A\00", align 1
@printf.str.4 = private unnamed_addr constant [12 x i8] c"Length: %d\0A\00", align 1
@printf.str.5 = private unnamed_addr constant [13 x i8] c"Capacity: %d\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %s = alloca %struct.String, align 8
  call void @_ZN6String4ctorEh(ptr nonnull %s, i8 72) #2
  %1 = call ptr @_ZN6String6getRawEv(ptr nonnull %s) #2
  %2 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.3, ptr %1)
  %3 = call i64 @_ZN6String9getLengthEv(ptr nonnull %s) #2
  %4 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.4, i64 %3)
  %5 = call i64 @_ZN6String11getCapacityEv(ptr nonnull %s) #2
  %6 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2, i64 %5)
  call void @_ZN6String6appendEPc(ptr nonnull %s, ptr nonnull @anon.string.0) #2
  %7 = call ptr @_ZN6String6getRawEv(ptr nonnull %s) #2
  %8 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.3, ptr %7)
  %9 = call i64 @_ZN6String9getLengthEv(ptr nonnull %s) #2
  %10 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.4, i64 %9)
  %11 = call i64 @_ZN6String11getCapacityEv(ptr nonnull %s) #2
  %12 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.5, i64 %11)
  call void @_ZN6String4dtorEv(ptr nonnull %s) #2
  ret i32 0
}

declare void @_ZN6String4ctorEh(ptr, i8) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare ptr @_ZN6String6getRawEv(ptr) local_unnamed_addr

declare i64 @_ZN6String9getLengthEv(ptr) local_unnamed_addr

declare i64 @_ZN6String11getCapacityEv(ptr) local_unnamed_addr

declare void @_ZN6String6appendEPc(ptr, ptr) local_unnamed_addr

declare void @_ZN6String4dtorEv(ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
