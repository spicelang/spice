; ModuleID = 'source.spice'
source_filename = "source.spice"

$_ZTS3Car = comdat any

$_ZTI3Car = comdat any

$_ZTV3Car = comdat any

@_ZTS3Car = weak_odr dso_local constant [5 x i8] c"3Car\00", comdat, align 4
@_ZTV8TypeInfo = external global ptr
@_ZTI9Driveable = external global ptr
@_ZTI3Car = weak_odr dso_local constant { ptr, ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS3Car, ptr @_ZTI9Driveable }, comdat, align 8
@_ZTV3Car = weak_odr dso_local unnamed_addr constant { [4 x ptr] } { [4 x ptr] [ptr null, ptr @_ZTI3Car, ptr @_ZN3Car5driveEi, ptr @_ZN3Car9isDrivingEv] }, comdat, align 8
@printf.str.0 = private unnamed_addr constant [15 x i8] c"Is driving: %d\00", align 4

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write) uwtable
define private void @_ZN3Car5driveEi(ptr nofree noundef nonnull writeonly align 8 captures(none) dereferenceable(16) initializes((8, 9)) %0, i32 %1) #0 {
  %driving.addr = getelementptr inbounds nuw i8, ptr %0, i64 8
  store i1 true, ptr %driving.addr, align 8
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read) uwtable
define private noundef zeroext i1 @_ZN3Car9isDrivingEv(ptr nofree noundef nonnull readonly align 8 captures(none) dereferenceable(16) %0) #1 {
  %driving.addr = getelementptr inbounds nuw i8, ptr %0, i64 8
  %2 = load i1, ptr %driving.addr, align 8
  ret i1 %2
}

; Function Attrs: mustprogress nofree noinline norecurse nounwind uwtable
define dso_local noundef i32 @main() local_unnamed_addr #2 {
  %1 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 noundef 1)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #3

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write) uwtable }
attributes #1 = { mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read) uwtable }
attributes #2 = { mustprogress nofree noinline norecurse nounwind uwtable }
attributes #3 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
