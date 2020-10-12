
function Component() {

}

Component.prototype.createOperations = function()
{
     try {
         // call the base create operations function
         component.createOperations();

     } catch (e) {
         console.log(e);
     }

}
