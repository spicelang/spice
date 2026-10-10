; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Local = type { ptr, i32 }
%struct.ImportedBox = type { ptr, i32 }

@_ZTS5Local = private constant [7 x i8] c"5Local\00", align 4
@_ZTV8TypeInfo = external global ptr
@_ZTI5Local = private constant { ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS5Local }, align 8
@_ZTV5Local = private unnamed_addr constant { [2 x ptr] } { [2 x ptr] [ptr null, ptr @_ZTI5Local] }, align 8
@printf.str.0 = private unnamed_addr constant [34 x i8] c"Ctor instance has vtable ptr: %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [40 x i8] c"Const literal: a=%d, vtable ptr ok: %d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [34 x i8] c"Literal: a=%d, vtable ptr ok: %d\0A\00", align 4
@printf.str.3 = private unnamed_addr constant [40 x i8] c"Empty literal: a=%d, vtable ptr ok: %d\0A\00", align 4
@_ZTV11ImportedBoxIiE = external global { [2 x ptr] }
@printf.str.4 = private unnamed_addr constant [53 x i8] c"Imported const literal: value=%d, vtable ptr ok: %d\0A\00", align 4
@printf.str.5 = private unnamed_addr constant [47 x i8] c"Imported literal: value=%d, vtable ptr ok: %d\0A\00", align 4

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define void @_ZN5Local4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  store ptr getelementptr inbounds ({ [2 x ptr] }, ptr @_ZTV5Local, i64 0, i32 0, i32 2), ptr %2, align 8
  %3 = getelementptr inbounds nuw %struct.Local, ptr %2, i32 0, i32 1
  store i32 0, ptr %3, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef ptr @_Z12getVTablePtrRK5Local(ptr noundef %0) #1 {
  %local = alloca ptr, align 8
  %vtablePtrAddr = alloca ptr, align 8
  store ptr %0, ptr %local, align 8
  %2 = load ptr, ptr %local, align 8
  store ptr %2, ptr %vtablePtrAddr, align 8
  %3 = load ptr, ptr %vtablePtrAddr, align 8
  %4 = load ptr, ptr %3, align 8
  ret ptr %4
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef ptr @_Z12getVTablePtrRK11ImportedBoxIiE(ptr noundef %0) #1 {
  %box = alloca ptr, align 8
  %vtablePtrAddr = alloca ptr, align 8
  store ptr %0, ptr %box, align 8
  %2 = load ptr, ptr %box, align 8
  store ptr %2, ptr %vtablePtrAddr, align 8
  %3 = load ptr, ptr %vtablePtrAddr, align 8
  %4 = load ptr, ptr %3, align 8
  ret ptr %4
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #2 {
  %ctorInstance = alloca %struct.Local, align 8
  %expectedVTablePtr = alloca ptr, align 8
  %constLiteral = alloca %struct.Local, align 8
  %two = alloca i32, align 4
  %literal = alloca %struct.Local, align 8
  %emptyLiteral = alloca %struct.Local, align 8
  %ctorBox = alloca %struct.ImportedBox, align 8
  %constBox = alloca %struct.ImportedBox, align 8
  %box = alloca %struct.ImportedBox, align 8
  call void @_ZN5Local4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %ctorInstance)
  %1 = call noundef ptr @_Z12getVTablePtrRK5Local(ptr noundef %ctorInstance)
  store ptr %1, ptr %expectedVTablePtr, align 8
  %2 = load ptr, ptr %expectedVTablePtr, align 8
  %3 = icmp ne ptr %2, null
  %4 = zext i1 %3 to i32
  %5 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %4)
  store %struct.Local { ptr getelementptr inbounds ({ [2 x ptr] }, ptr @_ZTV5Local, i64 0, i32 0, i32 2), i32 1 }, ptr %constLiteral, align 8
  %a.addr = getelementptr inbounds %struct.Local, ptr %constLiteral, i64 0, i32 1
  %6 = load i32, ptr %a.addr, align 4
  %7 = call noundef ptr @_Z12getVTablePtrRK5Local(ptr noundef %constLiteral)
  %8 = load ptr, ptr %expectedVTablePtr, align 8
  %9 = icmp eq ptr %7, %8
  %10 = zext i1 %9 to i32
  %11 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %6, i32 noundef %10)
  store i32 2, ptr %two, align 4
  store ptr getelementptr inbounds ({ [2 x ptr] }, ptr @_ZTV5Local, i64 0, i32 0, i32 2), ptr %literal, align 8
  %12 = load i32, ptr %two, align 4
  %13 = getelementptr inbounds nuw %struct.Local, ptr %literal, i32 0, i32 1
  store i32 %12, ptr %13, align 4
  %a.addr1 = getelementptr inbounds %struct.Local, ptr %literal, i64 0, i32 1
  %14 = load i32, ptr %a.addr1, align 4
  %15 = call noundef ptr @_Z12getVTablePtrRK5Local(ptr noundef %literal)
  %16 = load ptr, ptr %expectedVTablePtr, align 8
  %17 = icmp eq ptr %15, %16
  %18 = zext i1 %17 to i32
  %19 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 noundef %14, i32 noundef %18)
  store %struct.Local { ptr getelementptr inbounds ({ [2 x ptr] }, ptr @_ZTV5Local, i64 0, i32 0, i32 2), i32 0 }, ptr %emptyLiteral, align 8
  %a.addr2 = getelementptr inbounds %struct.Local, ptr %emptyLiteral, i64 0, i32 1
  %20 = load i32, ptr %a.addr2, align 4
  %21 = call noundef ptr @_Z12getVTablePtrRK5Local(ptr noundef %emptyLiteral)
  %22 = load ptr, ptr %expectedVTablePtr, align 8
  %23 = icmp eq ptr %21, %22
  %24 = zext i1 %23 to i32
  %25 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i32 noundef %20, i32 noundef %24)
  call void @_ZN11ImportedBoxIiE4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %ctorBox)
  store %struct.ImportedBox { ptr getelementptr inbounds ({ [2 x ptr] }, ptr @_ZTV11ImportedBoxIiE, i64 0, i32 0, i32 2), i32 3 }, ptr %constBox, align 8
  %value.addr = getelementptr inbounds %struct.ImportedBox, ptr %constBox, i64 0, i32 1
  %26 = load i32, ptr %value.addr, align 4
  %27 = call noundef ptr @_Z12getVTablePtrRK11ImportedBoxIiE(ptr noundef %constBox)
  %28 = call noundef ptr @_Z12getVTablePtrRK11ImportedBoxIiE(ptr noundef %ctorBox)
  %29 = icmp eq ptr %27, %28
  %30 = zext i1 %29 to i32
  %31 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i32 noundef %26, i32 noundef %30)
  store ptr getelementptr inbounds ({ [2 x ptr] }, ptr @_ZTV11ImportedBoxIiE, i64 0, i32 0, i32 2), ptr %box, align 8
  %32 = load i32, ptr %two, align 4
  %33 = getelementptr inbounds nuw %struct.ImportedBox, ptr %box, i32 0, i32 1
  store i32 %32, ptr %33, align 4
  %value.addr3 = getelementptr inbounds %struct.ImportedBox, ptr %box, i64 0, i32 1
  %34 = load i32, ptr %value.addr3, align 4
  %35 = call noundef ptr @_Z12getVTablePtrRK11ImportedBoxIiE(ptr noundef %box)
  %36 = call noundef ptr @_Z12getVTablePtrRK11ImportedBoxIiE(ptr noundef %ctorBox)
  %37 = icmp eq ptr %35, %36
  %38 = zext i1 %37 to i32
  %39 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.5, i32 noundef %34, i32 noundef %38)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #3

declare void @_ZN11ImportedBoxIiE4ctorEv(ptr noundef nonnull align 8 dereferenceable(16))

attributes #0 = { mustprogress noinline nounwind optnone uwtable }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #3 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
