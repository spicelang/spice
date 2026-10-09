; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Node = type { i32, ptr, ptr }
%struct.LeafSum = type { %interface.IVisitor, i32 }
%interface.IVisitor = type { ptr }
%struct.NodeCounter = type { %interface.IVisitor }

@_ZTS8IVisitor = private constant [10 x i8] c"8IVisitor\00", align 4
@_ZTV8TypeInfo = external global ptr
@_ZTI8IVisitor = private constant { ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS8IVisitor }, align 8
@_ZTV8IVisitor = private unnamed_addr constant { [6 x ptr] } { [6 x ptr] [ptr null, ptr @_ZTI8IVisitor, ptr null, ptr null, ptr null, ptr null] }, align 8
@printf.str.0 = private unnamed_addr constant [17 x i8] c"Unnamed visitor\0A\00", align 4
@_ZTS7LeafSum = private constant [9 x i8] c"7LeafSum\00", align 4
@_ZTI7LeafSum = private constant { ptr, ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS7LeafSum, ptr @_ZTI8IVisitor }, align 8
@_ZTV7LeafSum = private unnamed_addr constant { [6 x ptr] } { [6 x ptr] [ptr null, ptr @_ZTI7LeafSum, ptr @_ZN8IVisitor9visitNodeEP4Node, ptr @_ZN7LeafSum9visitLeafEP4Node, ptr @_ZN8IVisitor13visitChildrenEP4Node, ptr @_ZN8IVisitor9printNameEv] }, align 8
@_ZTS11NodeCounter = private constant [14 x i8] c"11NodeCounter\00", align 4
@_ZTI11NodeCounter = private constant { ptr, ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS11NodeCounter, ptr @_ZTI8IVisitor }, align 8
@_ZTV11NodeCounter = private unnamed_addr constant { [6 x ptr] } { [6 x ptr] [ptr null, ptr @_ZTI11NodeCounter, ptr @_ZN8IVisitor9visitNodeEP4Node, ptr @_ZN8IVisitor9visitLeafEP4Node, ptr @_ZN11NodeCounter13visitChildrenEP4Node, ptr @_ZN11NodeCounter9printNameEv] }, align 8
@printf.str.1 = private unnamed_addr constant [14 x i8] c"Node counter\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [14 x i8] c"Leaf sum: %d\0A\00", align 4
@printf.str.3 = private unnamed_addr constant [20 x i8] c"Visited leaves: %d\0A\00", align 4
@printf.str.4 = private unnamed_addr constant [17 x i8] c"Inner nodes: %d\0A\00", align 4
@printf.str.5 = private unnamed_addr constant [14 x i8] c"Leaf sum: %d\0A\00", align 4
@printf.str.6 = private unnamed_addr constant [14 x i8] c"Leaf sum: %d\0A\00", align 4
@printf.str.7 = private unnamed_addr constant [20 x i8] c"Visited leaves: %d\0A\00", align 4
@printf.str.8 = private unnamed_addr constant [17 x i8] c"Inner nodes: %d\0A\00", align 4
@printf.str.9 = private unnamed_addr constant [10 x i8] c"Leaf: %d\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_ZN8IVisitor9visitNodeEP4Node(ptr noundef nonnull align 8 dereferenceable(8) %0, ptr noundef align 8 %1) #0 {
  %this = alloca ptr, align 8
  %node = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %node, align 8
  %3 = load ptr, ptr %node, align 8
  %left.addr = getelementptr inbounds %struct.Node, ptr %3, i64 0, i32 1
  %4 = load ptr, ptr %left.addr, align 8
  %5 = icmp eq ptr %4, null
  br i1 %5, label %land.1.L16C8, label %land.exit.L16C8

land.1.L16C8:                                     ; preds = %2
  %6 = load ptr, ptr %node, align 8
  %right.addr = getelementptr inbounds %struct.Node, ptr %6, i64 0, i32 2
  %7 = load ptr, ptr %right.addr, align 8
  %8 = icmp eq ptr %7, null
  br label %land.exit.L16C8

land.exit.L16C8:                                  ; preds = %land.1.L16C8, %2
  %land_phi = phi i1 [ %5, %2 ], [ %8, %land.1.L16C8 ]
  br i1 %land_phi, label %if.then.L16, label %if.exit.L16

if.then.L16:                                      ; preds = %land.exit.L16C8
  %9 = load ptr, ptr %this, align 8
  %10 = load ptr, ptr %node, align 8
  %vtable.addr = load ptr, ptr %9, align 8
  %vfct.addr = getelementptr inbounds ptr, ptr %vtable.addr, i64 1
  %fct = load ptr, ptr %vfct.addr, align 8
  %11 = call noundef i32 %fct(ptr noundef nonnull align 8 dereferenceable(8) %9, ptr noundef align 8 %10)
  ret i32 %11

if.exit.L16:                                      ; preds = %land.exit.L16C8
  %12 = load ptr, ptr %this, align 8
  %13 = load ptr, ptr %node, align 8
  %vtable.addr1 = load ptr, ptr %12, align 8
  %vfct.addr2 = getelementptr inbounds ptr, ptr %vtable.addr1, i64 2
  %fct3 = load ptr, ptr %vfct.addr2, align 8
  %14 = call noundef i32 %fct3(ptr noundef nonnull align 8 dereferenceable(8) %12, ptr noundef align 8 %13)
  ret i32 %14
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_ZN8IVisitor9visitLeafEP4Node(ptr noundef nonnull align 8 dereferenceable(8) %0, ptr noundef align 8 %1) #0 {
  %this = alloca ptr, align 8
  %_node = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %_node, align 8
  ret i32 0
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_ZN8IVisitor13visitChildrenEP4Node(ptr noundef nonnull align 8 dereferenceable(8) %0, ptr noundef align 8 %1) #0 {
  %this = alloca ptr, align 8
  %node = alloca ptr, align 8
  %sum = alloca i32, align 4
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %node, align 8
  store i32 0, ptr %sum, align 4
  %3 = load ptr, ptr %node, align 8
  %left.addr = getelementptr inbounds %struct.Node, ptr %3, i64 0, i32 1
  %4 = load ptr, ptr %left.addr, align 8
  %5 = icmp ne ptr %4, null
  br i1 %5, label %if.then.L28, label %if.exit.L28

if.then.L28:                                      ; preds = %2
  %6 = load ptr, ptr %this, align 8
  %7 = load ptr, ptr %node, align 8
  %left.addr1 = getelementptr inbounds %struct.Node, ptr %7, i64 0, i32 1
  %8 = load ptr, ptr %left.addr1, align 8
  %vtable.addr = load ptr, ptr %6, align 8
  %vfct.addr = getelementptr inbounds ptr, ptr %vtable.addr, i64 0
  %fct = load ptr, ptr %vfct.addr, align 8
  %9 = call noundef i32 %fct(ptr noundef nonnull align 8 dereferenceable(8) %6, ptr noundef align 8 %8)
  %10 = load i32, ptr %sum, align 4
  %11 = add nsw i32 %10, %9
  store i32 %11, ptr %sum, align 4
  br label %if.exit.L28

if.exit.L28:                                      ; preds = %if.then.L28, %2
  %12 = load ptr, ptr %node, align 8
  %right.addr = getelementptr inbounds %struct.Node, ptr %12, i64 0, i32 2
  %13 = load ptr, ptr %right.addr, align 8
  %14 = icmp ne ptr %13, null
  br i1 %14, label %if.then.L31, label %if.exit.L31

if.then.L31:                                      ; preds = %if.exit.L28
  %15 = load ptr, ptr %this, align 8
  %16 = load ptr, ptr %node, align 8
  %right.addr2 = getelementptr inbounds %struct.Node, ptr %16, i64 0, i32 2
  %17 = load ptr, ptr %right.addr2, align 8
  %vtable.addr3 = load ptr, ptr %15, align 8
  %vfct.addr4 = getelementptr inbounds ptr, ptr %vtable.addr3, i64 0
  %fct5 = load ptr, ptr %vfct.addr4, align 8
  %18 = call noundef i32 %fct5(ptr noundef nonnull align 8 dereferenceable(8) %15, ptr noundef align 8 %17)
  %19 = load i32, ptr %sum, align 4
  %20 = add nsw i32 %19, %18
  store i32 %20, ptr %sum, align 4
  br label %if.exit.L31

if.exit.L31:                                      ; preds = %if.then.L31, %if.exit.L28
  %21 = load i32, ptr %sum, align 4
  ret i32 %21
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN8IVisitor9printNameEv(ptr noundef nonnull align 8 dereferenceable(8) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN7LeafSum4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  store ptr getelementptr inbounds ({ [6 x ptr] }, ptr @_ZTV7LeafSum, i64 0, i32 0, i32 2), ptr %2, align 8
  %3 = getelementptr inbounds nuw %struct.LeafSum, ptr %2, i32 0, i32 1
  store i32 0, ptr %3, align 4
  %4 = load ptr, ptr %this, align 8
  %visitedLeaves.addr = getelementptr inbounds %struct.LeafSum, ptr %4, i64 0, i32 1
  store i32 0, ptr %visitedLeaves.addr, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_ZN7LeafSum9visitLeafEP4Node(ptr noundef nonnull align 8 dereferenceable(16) %0, ptr noundef align 8 %1) #0 {
  %this = alloca ptr, align 8
  %node = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %node, align 8
  %3 = load ptr, ptr %this, align 8
  %visitedLeaves.addr = getelementptr inbounds %struct.LeafSum, ptr %3, i64 0, i32 1
  %4 = load i32, ptr %visitedLeaves.addr, align 4
  %5 = add nsw i32 %4, 1
  store i32 %5, ptr %visitedLeaves.addr, align 4
  %6 = load ptr, ptr %node, align 8
  %value.addr = getelementptr inbounds %struct.Node, ptr %6, i64 0, i32 0
  %7 = load i32, ptr %value.addr, align 4
  ret i32 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define void @_ZN11NodeCounter4ctorEv(ptr noundef nonnull align 8 dereferenceable(8) %0) #2 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  store ptr getelementptr inbounds ({ [6 x ptr] }, ptr @_ZTV11NodeCounter, i64 0, i32 0, i32 2), ptr %2, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_ZN11NodeCounter13visitChildrenEP4Node(ptr noundef nonnull align 8 dereferenceable(8) %0, ptr noundef align 8 %1) #0 {
  %this = alloca ptr, align 8
  %node = alloca ptr, align 8
  %sum = alloca i32, align 4
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %node, align 8
  store i32 1, ptr %sum, align 4
  %3 = load ptr, ptr %node, align 8
  %left.addr = getelementptr inbounds %struct.Node, ptr %3, i64 0, i32 1
  %4 = load ptr, ptr %left.addr, align 8
  %5 = icmp ne ptr %4, null
  br i1 %5, label %if.then.L60, label %if.exit.L60

if.then.L60:                                      ; preds = %2
  %6 = load ptr, ptr %this, align 8
  %7 = load ptr, ptr %node, align 8
  %left.addr1 = getelementptr inbounds %struct.Node, ptr %7, i64 0, i32 1
  %8 = load ptr, ptr %left.addr1, align 8
  %9 = call noundef i32 @_ZN8IVisitor9visitNodeEP4Node(ptr noundef nonnull align 8 dereferenceable(8) %6, ptr noundef align 8 %8)
  %10 = load i32, ptr %sum, align 4
  %11 = add nsw i32 %10, %9
  store i32 %11, ptr %sum, align 4
  br label %if.exit.L60

if.exit.L60:                                      ; preds = %if.then.L60, %2
  %12 = load ptr, ptr %node, align 8
  %right.addr = getelementptr inbounds %struct.Node, ptr %12, i64 0, i32 2
  %13 = load ptr, ptr %right.addr, align 8
  %14 = icmp ne ptr %13, null
  br i1 %14, label %if.then.L63, label %if.exit.L63

if.then.L63:                                      ; preds = %if.exit.L60
  %15 = load ptr, ptr %this, align 8
  %16 = load ptr, ptr %node, align 8
  %right.addr2 = getelementptr inbounds %struct.Node, ptr %16, i64 0, i32 2
  %17 = load ptr, ptr %right.addr2, align 8
  %18 = call noundef i32 @_ZN8IVisitor9visitNodeEP4Node(ptr noundef nonnull align 8 dereferenceable(8) %15, ptr noundef align 8 %17)
  %19 = load i32, ptr %sum, align 4
  %20 = add nsw i32 %19, %18
  store i32 %20, ptr %sum, align 4
  br label %if.exit.L63

if.exit.L63:                                      ; preds = %if.then.L63, %if.exit.L60
  %21 = load i32, ptr %sum, align 4
  ret i32 %21
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN11NodeCounter9printNameEv(ptr noundef nonnull align 8 dereferenceable(8) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_Z9visitTreeP8IVisitorP4Node(ptr noundef align 8 %0, ptr noundef align 8 %1) #0 {
  %visitor = alloca ptr, align 8
  %root = alloca ptr, align 8
  store ptr %0, ptr %visitor, align 8
  store ptr %1, ptr %root, align 8
  %3 = load ptr, ptr %visitor, align 8
  %vtable.addr = load ptr, ptr %3, align 8
  %vfct.addr = getelementptr inbounds ptr, ptr %vtable.addr, i64 3
  %fct = load ptr, ptr %vfct.addr, align 8
  call void %fct(ptr noundef nonnull align 8 dereferenceable(8) %3)
  %4 = load ptr, ptr %visitor, align 8
  %5 = load ptr, ptr %root, align 8
  %vtable.addr1 = load ptr, ptr %4, align 8
  %vfct.addr2 = getelementptr inbounds ptr, ptr %vtable.addr1, i64 0
  %fct3 = load ptr, ptr %vfct.addr2, align 8
  %6 = call noundef i32 %fct3(ptr noundef nonnull align 8 dereferenceable(8) %4, ptr noundef align 8 %5)
  ret i32 %6
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #3 {
  %leaf1 = alloca %struct.Node, align 8
  %leaf2 = alloca %struct.Node, align 8
  %leaf3 = alloca %struct.Node, align 8
  %inner = alloca %struct.Node, align 8
  %root = alloca %struct.Node, align 8
  %leafSum = alloca %struct.LeafSum, align 8
  %nodeCounter = alloca %struct.NodeCounter, align 8
  store %struct.Node { i32 1, ptr null, ptr null }, ptr %leaf1, align 8
  store %struct.Node { i32 2, ptr null, ptr null }, ptr %leaf2, align 8
  store %struct.Node { i32 3, ptr null, ptr null }, ptr %leaf3, align 8
  store i32 4, ptr %inner, align 4
  %1 = getelementptr inbounds nuw %struct.Node, ptr %inner, i32 0, i32 1
  store ptr %leaf1, ptr %1, align 8
  %2 = getelementptr inbounds nuw %struct.Node, ptr %inner, i32 0, i32 2
  store ptr %leaf2, ptr %2, align 8
  store i32 5, ptr %root, align 4
  %3 = getelementptr inbounds nuw %struct.Node, ptr %root, i32 0, i32 1
  store ptr %inner, ptr %3, align 8
  %4 = getelementptr inbounds nuw %struct.Node, ptr %root, i32 0, i32 2
  store ptr %leaf3, ptr %4, align 8
  call void @_ZN7LeafSum4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %leafSum)
  %5 = call noundef i32 @_Z9visitTreeP8IVisitorP4Node(ptr noundef align 8 %leafSum, ptr noundef align 8 %root)
  %6 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 noundef %5)
  %visitedLeaves.addr = getelementptr inbounds %struct.LeafSum, ptr %leafSum, i64 0, i32 1
  %7 = load i32, ptr %visitedLeaves.addr, align 4
  %8 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i32 noundef %7)
  call void @_ZN11NodeCounter4ctorEv(ptr noundef nonnull align 8 dereferenceable(8) %nodeCounter)
  %9 = call noundef i32 @_Z9visitTreeP8IVisitorP4Node(ptr noundef align 8 %nodeCounter, ptr noundef align 8 %root)
  %10 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i32 noundef %9)
  call void @_ZN8IVisitor9printNameEv(ptr noundef nonnull align 8 dereferenceable(16) %leafSum)
  %11 = call noundef i32 @_ZN8IVisitor9visitNodeEP4Node(ptr noundef nonnull align 8 dereferenceable(16) %leafSum, ptr noundef align 8 %inner)
  %12 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.5, i32 noundef %11)
  %13 = call noundef i32 @_ZN8IVisitor13visitChildrenEP4Node(ptr noundef nonnull align 8 dereferenceable(16) %leafSum, ptr noundef align 8 %root)
  %14 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.6, i32 noundef %13)
  %visitedLeaves.addr1 = getelementptr inbounds %struct.LeafSum, ptr %leafSum, i64 0, i32 1
  %15 = load i32, ptr %visitedLeaves.addr1, align 4
  %16 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.7, i32 noundef %15)
  %17 = call noundef i32 @_ZN8IVisitor9visitNodeEP4Node(ptr noundef nonnull align 8 dereferenceable(8) %nodeCounter, ptr noundef align 8 %inner)
  %18 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.8, i32 noundef %17)
  %19 = call noundef i32 @_ZN8IVisitor9visitLeafEP4Node(ptr noundef nonnull align 8 dereferenceable(8) %nodeCounter, ptr noundef align 8 %leaf3)
  %20 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.9, i32 noundef %19)
  ret i32 0
}

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { mustprogress noinline nounwind optnone uwtable }
attributes #3 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev [host] (https://github.com/spicelang/spice)"}
