; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Car = type { %interface.Driveable, i1 }
%interface.Driveable = type { ptr }

$_ZTS3Car = comdat any

$_ZTI3Car = comdat any

$_ZTV3Car = comdat any

@_ZTS3Car = dso_local constant [5 x i8] c"3Car\00", comdat, align 1
@_ZTV8TypeInfo = external global ptr
@_ZTI9Driveable = external global ptr
@_ZTI3Car = dso_local constant { ptr, ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS3Car, ptr @_ZTI9Driveable }, comdat, align 8
@_ZTV3Car = dso_local unnamed_addr constant { [4 x ptr] } { [4 x ptr] [ptr null, ptr @_ZTI3Car, ptr @_ZN3Car5driveEi, ptr @_ZN3Car9isDrivingEv] }, comdat, align 8
@printf.str.0 = private unnamed_addr constant [15 x i8] c"Is driving: %d\00", align 1

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write)
define private fastcc void @_ZN3Car4ctorEv(ptr nocapture noundef nonnull writeonly align 8 dereferenceable(16) initializes((0, 9)) %0) unnamed_addr #0 {
  store ptr getelementptr inbounds nuw (i8, ptr @_ZTV3Car, i64 16), ptr %0, align 8
  %2 = getelementptr inbounds nuw i8, ptr %0, i64 8
  store i1 false, ptr %2, align 8
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write)
define private void @_ZN3Car5driveEi(ptr nocapture noundef nonnull writeonly align 8 dereferenceable(16) initializes((8, 9)) %0, i32 %1) #0 {
  %driving_addr = getelementptr inbounds nuw i8, ptr %0, i64 8
  store i1 true, ptr %driving_addr, align 8
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define private i1 @_ZN3Car9isDrivingEv(ptr nocapture noundef nonnull readonly align 8 dereferenceable(16) %0) #1 {
  %driving_addr = getelementptr inbounds nuw i8, ptr %0, i64 8
  %2 = load i1, ptr %driving_addr, align 8
  ret i1 %2
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #2 {
  %car = alloca %struct.Car, align 8
  call fastcc void @_ZN3Car4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %car) #4
  %vtable.addr = load ptr, ptr %car, align 8
  %fct = load ptr, ptr %vtable.addr, align 8
  call void %fct(ptr noundef nonnull align 8 dereferenceable(8) %car, i32 12) #4
  %vtable.addr1 = load ptr, ptr %car, align 8
  %vfct.addr2 = getelementptr inbounds nuw i8, ptr %vtable.addr1, i64 8
  %fct3 = load ptr, ptr %vfct.addr2, align 8
  %1 = call i1 %fct3(ptr noundef nonnull align 8 dereferenceable(8) %car) #4
  %2 = zext i1 %1 to i32
  %3 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 %2)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #3

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write) }
attributes #1 = { mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read) }
attributes #2 = { noinline nounwind optnone uwtable }
attributes #3 = { nofree nounwind }
attributes #4 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
