; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.String = type { ptr, i64, i64 }

@anon.string.0 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.1 = private unnamed_addr constant [5 x i8] c" Hi!\00", align 1
@printf.str.1 = private unnamed_addr constant [8 x i8] c"S1: %s\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [8 x i8] c"S2: %s\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %s1 = alloca %struct.String, align 8
  %s2 = alloca %struct.String, align 8
  %1 = alloca ptr, align 8
  call void @_ZN6String4ctorEPc(ptr nonnull %s1, ptr nonnull @anon.string.0) #2
  %.fca.0.load2 = load ptr, ptr %s1, align 8
  %.fca.0.insert3 = insertvalue %struct.String poison, ptr %.fca.0.load2, 0
  %.fca.1.gep4 = getelementptr inbounds %struct.String, ptr %s1, i64 0, i32 1
  %.fca.1.load5 = load i64, ptr %.fca.1.gep4, align 8
  %.fca.1.insert6 = insertvalue %struct.String %.fca.0.insert3, i64 %.fca.1.load5, 1
  %.fca.2.gep7 = getelementptr inbounds %struct.String, ptr %s1, i64 0, i32 2
  %.fca.2.load8 = load i64, ptr %.fca.2.gep7, align 8
  %.fca.2.insert9 = insertvalue %struct.String %.fca.1.insert6, i64 %.fca.2.load8, 2
  %2 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, %struct.String %.fca.2.insert9)
  call void @_ZN6String4ctorERK6String(ptr nonnull %s2, ptr nonnull %s1) #2
  store ptr @anon.string.1, ptr %1, align 8
  call void @_Z12op.plusequalR6StringRKPc(ptr nonnull %s2, ptr nonnull %1) #2
  %.fca.0.load11 = load ptr, ptr %s1, align 8
  %.fca.0.insert12 = insertvalue %struct.String poison, ptr %.fca.0.load11, 0
  %.fca.1.load14 = load i64, ptr %.fca.1.gep4, align 8
  %.fca.1.insert15 = insertvalue %struct.String %.fca.0.insert12, i64 %.fca.1.load14, 1
  %.fca.2.load17 = load i64, ptr %.fca.2.gep7, align 8
  %.fca.2.insert18 = insertvalue %struct.String %.fca.1.insert15, i64 %.fca.2.load17, 2
  %3 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, %struct.String %.fca.2.insert18)
  %.fca.0.load = load ptr, ptr %s2, align 8
  %.fca.0.insert = insertvalue %struct.String poison, ptr %.fca.0.load, 0
  %.fca.1.gep = getelementptr inbounds %struct.String, ptr %s2, i64 0, i32 1
  %.fca.1.load = load i64, ptr %.fca.1.gep, align 8
  %.fca.1.insert = insertvalue %struct.String %.fca.0.insert, i64 %.fca.1.load, 1
  %.fca.2.gep = getelementptr inbounds %struct.String, ptr %s2, i64 0, i32 2
  %.fca.2.load = load i64, ptr %.fca.2.gep, align 8
  %.fca.2.insert = insertvalue %struct.String %.fca.1.insert, i64 %.fca.2.load, 2
  %4 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2, %struct.String %.fca.2.insert)
  call void @_ZN6String4dtorEv(ptr nonnull %s2) #2
  call void @_ZN6String4dtorEv(ptr nonnull %s1) #2
  ret i32 0
}

declare void @_ZN6String4ctorEPc(ptr, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare void @_ZN6String4ctorERK6String(ptr, ptr) local_unnamed_addr

declare void @_Z12op.plusequalR6StringRKPc(ptr, ptr) local_unnamed_addr

declare void @_ZN6String4dtorEv(ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
